#include "asgn2_helper_funcs.h"
#include "connection.h"
#include "debug.h"
#include "response.h"
#include "request.h"
#include "queue.h"
#include "rwlock.h"
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pthread.h>
#include <getopt.h>
#include <stdbool.h>
#include <time.h>
#include <poll.h>

// Global variables
static queue_t *request_queue;
static rwlock_t *log_lock;
static pthread_mutex_t *file_locks_mutex;
static pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t queue_cond = PTHREAD_COND_INITIALIZER;

static struct file_lock {
    char *path;
    pthread_rwlock_t rwlock;
    int ref_count;
    pthread_mutex_t ref_mutex;
    bool is_deleted;
} **file_locks;
static size_t file_locks_size = 0;
static int num_threads = 4;

typedef struct {
    int connfd;
    char *request_id;
} work_item_t;

static volatile sig_atomic_t keep_running = 1;

// Function prototypes
void handle_connection(int);
void handle_get(conn_t *);
void handle_put(conn_t *);
void handle_unsupported(conn_t *);
void *worker_thread(void *arg);
void log_request(const char *method, const char *uri, int status_code, const char *request_id);
pthread_rwlock_t *get_file_rwlock(const char *path, bool is_write);
void release_file_rwlock(const char *path, bool is_write);
bool queue_push_with_signal(queue_t *q, void *elem);

void handle_signal(int sig) {
    if (sig == SIGTERM) {
        keep_running = 0;
    }
}

// Timed lock helper function
int timed_rwlock_wrlock(pthread_rwlock_t *rwlock, int timeout_ms) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);

    ts.tv_nsec += (timeout_ms % 1000) * 1000000;
    ts.tv_sec += timeout_ms / 1000;

    if (ts.tv_nsec >= 1000000000) {
        ts.tv_sec++;
        ts.tv_nsec -= 1000000000;
    }

    return pthread_rwlock_timedwrlock(rwlock, &ts);
}

pthread_rwlock_t *get_file_rwlock(const char *path, bool is_write) {
    if (!path) {
        return NULL;
    }

    pthread_mutex_lock(file_locks_mutex);

    // Search for existing lock
    for (size_t i = 0; i < file_locks_size; i++) {
        if (file_locks[i] && strcmp(file_locks[i]->path, path) == 0) {
            pthread_mutex_lock(&file_locks[i]->ref_mutex);

            // More robust handling of write lock
            if (is_write) {
                // Instead of busy waiting, just increment ref count
                // This allows other read operations to proceed
                file_locks[i]->ref_count++;
                pthread_mutex_unlock(&file_locks[i]->ref_mutex);
                pthread_mutex_unlock(file_locks_mutex);

                // Try to acquire write lock with a timeout
                int lock_result = timed_rwlock_wrlock(&file_locks[i]->rwlock, 1000); // 1-second timeout
                if (lock_result != 0) {
                    pthread_mutex_lock(file_locks_mutex);
                    pthread_mutex_lock(&file_locks[i]->ref_mutex);
                    file_locks[i]->ref_count--;
                    pthread_mutex_unlock(&file_locks[i]->ref_mutex);
                    pthread_mutex_unlock(file_locks_mutex);

                    // Retry or handle timeout more gracefully
                    return NULL; // Or implement retry logic with backoff
                }

                return &file_locks[i]->rwlock;
            }

            file_locks[i]->ref_count++;
            pthread_mutex_unlock(&file_locks[i]->ref_mutex);
            pthread_mutex_unlock(file_locks_mutex);

            // Acquire appropriate lock
            if (is_write) {
                pthread_rwlock_wrlock(&file_locks[i]->rwlock);
            } else {
                pthread_rwlock_rdlock(&file_locks[i]->rwlock);
            }

            return &file_locks[i]->rwlock;
        }
    }

    // Create new lock
    struct file_lock *new_lock = malloc(sizeof(struct file_lock));
    if (!new_lock) {
        pthread_mutex_unlock(file_locks_mutex);
        err(1, "Failed to allocate file lock");
    }

    new_lock->path = strdup(path);
    pthread_rwlock_init(&new_lock->rwlock, NULL);
    pthread_mutex_init(&new_lock->ref_mutex, NULL);
    new_lock->ref_count = 1;
    new_lock->is_deleted = false;

    // Find empty slot or expand array
    size_t index = file_locks_size;
    for (size_t i = 0; i < file_locks_size; i++) {
        if (file_locks[i] == NULL) {
            index = i;
            break;
        }
    }

    if (index == file_locks_size) {
        size_t new_size = file_locks_size == 0 ? 8 : file_locks_size * 2;
        file_locks = realloc(file_locks, new_size * sizeof(struct file_lock *));
        if (!file_locks) {
            pthread_mutex_unlock(file_locks_mutex);
            err(1, "Failed to reallocate file locks");
        }
        memset(file_locks + file_locks_size, 0,
            (new_size - file_locks_size) * sizeof(struct file_lock *));
        file_locks_size = new_size;
    }

    file_locks[index] = new_lock;
    pthread_mutex_unlock(file_locks_mutex);

    // Acquire read or write lock
    if (is_write) {
        pthread_rwlock_wrlock(&new_lock->rwlock);
    } else {
        pthread_rwlock_rdlock(&new_lock->rwlock);
    }
    return &new_lock->rwlock;
}

void release_file_rwlock(const char *path, bool is_write __attribute__((unused))) {
    if (!path) {
        return;
    }

    pthread_mutex_lock(file_locks_mutex);

    for (size_t i = 0; i < file_locks_size; i++) {
        if (file_locks[i] && strcmp(file_locks[i]->path, path) == 0) {
            pthread_rwlock_unlock(&file_locks[i]->rwlock);

            pthread_mutex_lock(&file_locks[i]->ref_mutex);
            file_locks[i]->ref_count--;
            bool should_cleanup = (file_locks[i]->ref_count == 0 && file_locks[i]->is_deleted);
            pthread_mutex_unlock(&file_locks[i]->ref_mutex);

            if (should_cleanup) {
                pthread_rwlock_destroy(&file_locks[i]->rwlock);
                pthread_mutex_destroy(&file_locks[i]->ref_mutex);
                free(file_locks[i]->path);
                free(file_locks[i]);
                file_locks[i] = NULL;
            }

            break;
        }
    }

    pthread_mutex_unlock(file_locks_mutex);
}

void log_request(const char *method, const char *uri, int status_code, const char *request_id) {
    writer_lock(log_lock);
    fprintf(stderr, "%s,%s,%d,%s\n", method, uri, status_code, request_id ? request_id : "0");
    writer_unlock(log_lock);
}

void *worker_thread(void *arg) {
    (void) arg;
    while (keep_running) {
        pthread_mutex_lock(&queue_mutex);

        // Instead of using queue_is_empty, attempt to pop and handle accordingly
        void *item;
        if (queue_pop(request_queue, &item)) {
            // Successfully popped an item
            pthread_mutex_unlock(&queue_mutex);

            work_item_t *work = (work_item_t *) item;
            handle_connection(work->connfd);
            close(work->connfd);
            free(work->request_id);
            free(work);
        } else {
            // If queue is empty, wait on condition variable with timeout
            if (keep_running) {
                struct timespec ts;
                clock_gettime(CLOCK_REALTIME, &ts);
                ts.tv_sec += 1;  // 1-second timeout
                
                int wait_result = pthread_cond_timedwait(&queue_cond, &queue_mutex, &ts);
                
                if (wait_result == ETIMEDOUT) {
                    // Timeout occurred, check keep_running
                    if (!keep_running) {
                        pthread_mutex_unlock(&queue_mutex);
                        break;
                    }
                }
            }
            
            pthread_mutex_unlock(&queue_mutex);
        }
    }
    return NULL;
}

void handle_get(conn_t *conn) {
    char *uri = conn_get_uri(conn);
    const Response_t *response = NULL;
    char *request_id = conn_get_header(conn, "Request-Id");

    pthread_rwlock_t *file_lock = get_file_rwlock(uri, false);
    if (!file_lock) {
        response = &RESPONSE_INTERNAL_SERVER_ERROR;
        log_request("GET", uri, response_get_code(response), request_id);
        conn_send_response(conn, response);
        return;
    }

    int fd = open(uri, O_RDONLY);
    if (fd < 0) {
        if (errno == EACCES) {
            response = &RESPONSE_FORBIDDEN;
        } else if (errno == ENOENT) {
            response = &RESPONSE_NOT_FOUND;
        } else {
            response = &RESPONSE_INTERNAL_SERVER_ERROR;
        }
        log_request("GET", uri, response_get_code(response), request_id);
        conn_send_response(conn, response);
        release_file_rwlock(uri, false);
        return;
    }

    struct stat st;
    if (fstat(fd, &st) < 0) {
        response = &RESPONSE_INTERNAL_SERVER_ERROR;
        log_request("GET", uri, response_get_code(response), request_id);
        conn_send_response(conn, response);
        close(fd);
        release_file_rwlock(uri, false);
        return;
    }

    if (S_ISDIR(st.st_mode)) {
        response = &RESPONSE_FORBIDDEN;
        log_request("GET", uri, response_get_code(response), request_id);
        conn_send_response(conn, response);
        close(fd);
        release_file_rwlock(uri, false);
        return;
    }

    response = conn_send_file(conn, fd, st.st_size);
    log_request("GET", uri, response ? response_get_code(response) : 200, request_id);
    if (response) {
        conn_send_response(conn, response);
    }
    close(fd);
    release_file_rwlock(uri, false);
}

void handle_put(conn_t *conn) {
    char *uri = conn_get_uri(conn);
    const Response_t *response = NULL;
    char *request_id = conn_get_header(conn, "Request-Id");

    // Create a temporary file to buffer the entire request
    char temp_filename[] = "/tmp/httpserver_XXXXXX";
    int temp_fd = mkstemp(temp_filename);
    if (temp_fd < 0) {
        response = &RESPONSE_INTERNAL_SERVER_ERROR;
        log_request("PUT", uri, response_get_code(response), request_id);
        conn_send_response(conn, response);
        return;
    }

    // Receive the entire file into the temporary file
    response = conn_recv_file(conn, temp_fd);
    if (response) {
        close(temp_fd);
        unlink(temp_filename);
        log_request("PUT", uri, response_get_code(response), request_id);
        conn_send_response(conn, response);
        return;
    }

    // Get file size
    // off_t file_size = lseek(temp_fd, 0, SEEK_END);
    lseek(temp_fd, 0, SEEK_SET);

    // Now acquire the lock and process the file
    pthread_rwlock_t *file_lock = get_file_rwlock(uri, true);
    if (!file_lock) {
        close(temp_fd);
        unlink(temp_filename);
        response = &RESPONSE_INTERNAL_SERVER_ERROR;
        log_request("PUT", uri, response_get_code(response), request_id);
        conn_send_response(conn, response);
        return;
    }

    // Open destination file
    bool file_exists = (access(uri, F_OK) == 0);
    int dest_fd = open(uri, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd < 0) {
        close(temp_fd);
        unlink(temp_filename);
        release_file_rwlock(uri, true);
        
        if (errno == EACCES || errno == EISDIR) {
            response = &RESPONSE_FORBIDDEN;
        } else {
            response = &RESPONSE_INTERNAL_SERVER_ERROR;
        }
        log_request("PUT", uri, response_get_code(response), request_id);
        conn_send_response(conn, response);
        return;
    }

    // Copy from temp file to destination
    char buffer[4096];
    ssize_t bytes_read;
    while ((bytes_read = read(temp_fd, buffer, sizeof(buffer))) > 0) {
        if (write(dest_fd, buffer, bytes_read) != bytes_read) {
            close(temp_fd);
            close(dest_fd);
            unlink(temp_filename);
            release_file_rwlock(uri, true);
            
            response = &RESPONSE_INTERNAL_SERVER_ERROR;
            log_request("PUT", uri, response_get_code(response), request_id);
            conn_send_response(conn, response);
            return;
        }
    }

    // Cleanup temporary file and file descriptors
    close(temp_fd);
    close(dest_fd);
    unlink(temp_filename);

    // Send appropriate response
    response = file_exists ? &RESPONSE_OK : &RESPONSE_CREATED;
    log_request("PUT", uri, response_get_code(response), request_id);
    conn_send_response(conn, response);

    // Release file lock
    release_file_rwlock(uri, true);
}

void handle_unsupported(conn_t *conn) {
    char *uri = conn_get_uri(conn);
    char *request_id = conn_get_header(conn, "Request-Id");
    log_request(request_get_str(conn_get_request(conn)), uri, 501, request_id);
    conn_send_response(conn, &RESPONSE_NOT_IMPLEMENTED);
}

void handle_connection(int connfd) {
    conn_t *conn = conn_new(connfd);
    const Response_t *res = conn_parse(conn);

    if (res != NULL) {
        char *uri = conn_get_uri(conn);
        char *request_id = conn_get_header(conn, "Request-Id");
        log_request(
            request_get_str(conn_get_request(conn)), uri, response_get_code(res), request_id);
        conn_send_response(conn, res);
    } else {
        const Request_t *req = conn_get_request(conn);
        if (req == &REQUEST_GET) {
            handle_get(conn);
        } else if (req == &REQUEST_PUT) {
            handle_put(conn);
        } else {
            handle_unsupported(conn);
        }
    }
    conn_delete(&conn);
}

bool queue_push_with_signal(queue_t *q, void *elem) {
    bool result = queue_push(q, elem);
    if (result) {
        pthread_cond_signal(&queue_cond); // Ensure this is triggered correctly
    }
    return result;
}


int main(int argc, char **argv) {
    int opt;
    while ((opt = getopt(argc, argv, "t:")) != -1) {
        switch (opt) {
        case 't':
            num_threads = atoi(optarg);
            if (num_threads <= 0) {
                fprintf(stderr, "Invalid number of threads\n");
                return EXIT_FAILURE;
            }
            break;
        default: fprintf(stderr, "Usage: %s [-t threads] <port>\n", argv[0]); return EXIT_FAILURE;
        }
    }

    if (optind >= argc) {
        fprintf(stderr, "Usage: %s [-t threads] <port>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *endptr = NULL;
    size_t port = (size_t) strtoull(argv[optind], &endptr, 10);
    if (endptr && *endptr != '\0') {
        fprintf(stderr, "Invalid Port\n");
        return EXIT_FAILURE;
    }
    if (port < 1 || port > 65535) {
        fprintf(stderr, "Invalid Port\n");
        return EXIT_FAILURE;
    }

    // Initialize signal handler
    signal(SIGPIPE, SIG_IGN);
    signal(SIGTERM, handle_signal);

    // Initialize synchronization primitives
    request_queue = queue_new(num_threads * 2);
    log_lock = rwlock_new(WRITERS, 1);
    file_locks_mutex = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(file_locks_mutex, NULL);
    file_locks = NULL;
    file_locks_size = 0;

    pthread_t *workers = malloc(num_threads * sizeof(pthread_t));

    // Create worker threads
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&workers[i], NULL, worker_thread, NULL);
    }

    // Initialize listener socket
    Listener_Socket sock;
    if (listener_init(&sock, port) < 0) {
        fprintf(stderr, "Failed to initialize listener\n");
        return EXIT_FAILURE;
    }

    // Main dispatcher loop
    while (keep_running) {
        int connfd = listener_accept(&sock);
        if (connfd < 0) {
            continue;
        }

        work_item_t *work = malloc(sizeof(work_item_t));
        work->connfd = connfd;
        work->request_id = NULL;

        if (!queue_push(request_queue, work)) {
            close(connfd);
            free(work);
        }
    }

    // Cleanup
    for (int i = 0; i < num_threads; i++) {
        pthread_join(workers[i], NULL);
    }

    // Clean up file locks
    for (size_t i = 0; i < file_locks_size; i++) {
        if (file_locks[i]) {
            pthread_rwlock_destroy(&file_locks[i]->rwlock);
            free(file_locks[i]->path);
            free(file_locks[i]);
        }
    }
    free(file_locks);
    pthread_mutex_destroy(file_locks_mutex);
    free(file_locks_mutex);

    free(workers);
    queue_delete(&request_queue);
    rwlock_delete(&log_lock);

    return EXIT_SUCCESS;
}
