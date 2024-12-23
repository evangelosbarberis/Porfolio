# Evangelos Barberis asign 4 - Multithreaded Httpsever CSE 130 UCSC

## Breif overview
Simple httpserver in C, that can handle concurrent get and put requests.

- Thread safe request processing
- Config Worker threads
- Atomic request logging
- file locking for concurrent files


### Below are some simple features
- Supports GET and PUT HTTP methods
- Audit logging to stderr
- Ensures request linearization and consistency


### Below are some simple limitations of the server
- Only GET and PUT methods supported
- Single request per connection
- File system constraints apply


### Files used
I was able to use all of the header functions provided for the assignment, as all I edited for the completion of this assignment was the httpserver.c file. Some of what I had issues with was executing the server with conflicting puts that cause pause, I ended up editing file support functions to help edit that error!