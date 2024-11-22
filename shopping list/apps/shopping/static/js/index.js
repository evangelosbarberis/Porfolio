"use strict";

let app = {};
app.data = {
    data: function() {
        return {
            items: [], //list of the shopping items, keeps an updated list
            new_item: "", // allows for a new item to be inputed
        };
    },
    computed: {
        // allows us to sort the items while keeping the unpurchased stuff on top
        // had the most issues with keeping items at top of the list without purchase!
        sorted_items() {
            return [...this.items].sort((a, b) => {
                if (a.is_purchased !== b.is_purchased) {
                    return a.is_purchased ? 1 : -1;
                }
                if (!a.is_purchased) {
                    return 0; 
                } else {
                    return 1; 
                }
            });
        }
    },
    methods: {
        // add the new item on the top of the lisr
        add_item() {
            if (!this.new_item.trim()) return;
            axios.post(add_item_url, {
                item_name: this.new_item
            }).then(response => {
                this.items.unshift({
                    id: response.data.id,
                    item_name: this.new_item,
                    is_purchased: false
                });
                this.new_item = "";
            });
        },
        // update the item's status of purchase
        update_item(item) {
            axios.post(update_item_url, {
                id: item.id,
                is_purchased: item.is_purchased
            }).then(() => {
                const updatedItems = [...this.items];
                const index = updatedItems.findIndex(i => i.id === item.id);
                if (index > -1) {
                    const updatedItem = updatedItems.splice(index, 1)[0];
                    if (updatedItem.is_purchased) {
                        updatedItems.push(updatedItem);
                    } else {
                        updatedItems.unshift(updatedItem);
                    }
                }
                this.items = updatedItems;
            });
        },
        // delete the item from the list
        delete_item(item) {
            axios.post(delete_item_url, {
                id: item.id
            }).then(() => {
                const idx = this.items.findIndex(i => i.id === item.id);
                if (idx > -1) {
                    this.items.splice(idx, 1);
                }
            });
        },
        // load the itial data from sever
        load_data() {
            axios.get(load_data_url).then(response => {
                this.items = response.data.items;
            });
        }
    }
};

app.vue = Vue.createApp(app.data).mount("#app");
app.load_data = function() {
    app.vue.load_data();
}
app.load_data();