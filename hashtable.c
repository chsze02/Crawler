#include "hashtable.h"
#include "set.h"
#include <stdlib.h>  // Include the header for malloc and free

// A simple hash function for demonstration purposes
unsigned long basicHash(const char *str, int num_slots) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++) != '\0') {
        hash = ((hash << 5) + hash) + c;  // hash * 33 + c
    }

    return hash % num_slots;
}

// Create a new (empty) hashtable; return NULL if error.
hashtable_t *hashtable_new(const int num_slots) {
    if (num_slots <= 0) {
        return NULL;  // Invalid number of slots
    }

    // Allocate memory for the hashtable structure
    hashtable_t *ht = (hashtable_t *)malloc(sizeof(hashtable_t));
    if (ht == NULL) {
        return NULL;  // Memory allocation failure
    }

    // Allocate memory for the array of sets
    ht->slots = (set_t **)malloc(sizeof(set_t *) * num_slots);
    if (ht->slots == NULL) {
        free(ht);
        return NULL;  // Memory allocation failure
    }

    // Initialize each slot with an empty set
    for (int i = 0; i < num_slots; i++) {
        ht->slots[i] = set_new();
        if (ht->slots[i] == NULL) {
            // Memory allocation failure, clean up and return NULL
            for (int j = 0; j < i; j++) {
                set_delete(ht->slots[j], NULL);
            }
            free(ht->slots);
            free(ht);
            return NULL;
        }
    }

    ht->num_slots = num_slots;
    return ht;
}

// Insert item, identified by key (string), into the given hashtable.
bool hashtable_insert(hashtable_t *ht, const char *key, void *item) {
    if (ht == NULL || key == NULL || item == NULL) {
        return false;  // Invalid parameters
    }

    // Hash the key to determine the slot
    unsigned long hash = basicHash(key, ht->num_slots);

    // Insert the item into the corresponding set
    return set_insert(ht->slots[hash], key, item);
}

// Return the item associated with the given key.
void *hashtable_find(hashtable_t *ht, const char *key) {
    if (ht == NULL || key == NULL) {
        return NULL;  // Invalid parameters
    }

    // Hash the key to determine the slot
    unsigned long hash = basicHash(key, ht->num_slots);

    // Find the item in the corresponding set
    return set_find(ht->slots[hash], key);
}

// Print the whole table; provide the output file and func to print each item.
void hashtable_print(hashtable_t *ht, FILE *fp,
                     void (*itemprint)(FILE *fp, const char *key, void *item)) {
    if (fp == NULL || ht == NULL) {
        return;  // Invalid parameters
    }

    // Iterate through each slot and print the set
    for (int i = 0; i < ht->num_slots; i++) {
        set_print(ht->slots[i], fp, itemprint);
    }
}

// Iterate over all items in the table; in undefined order.
void hashtable_iterate(hashtable_t *ht, void *arg,
                       void (*itemfunc)(void *arg, const char *key, void *item)) {
    if (ht == NULL || itemfunc == NULL) {
        return;  // Invalid parameters
    }

    // Iterate through each slot and call itemfunc on each item
    for (int i = 0; i < ht->num_slots; i++) {
        set_iterate(ht->slots[i], arg, itemfunc);
    }
}

// Delete the whole hashtable; ignore NULL ht.
void hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item)) {
    if (ht == NULL) {
        return;  // Nothing to delete
    }

    // Iterate through each slot and delete the set
    for (int i = 0; i < ht->num_slots; i++) {
        set_delete(ht->slots[i], itemdelete);
    }

    // Free the array of sets and the hashtable structure
    free(ht->slots);
    free(ht);
}

