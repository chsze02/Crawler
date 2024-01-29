#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "set.h"

// Define a structure for each node in the set
typedef struct setnode {
    char *key;
    void *item;
    struct setnode *next;
} setnode_t;

// Define the set structure
struct set {
    setnode_t *head;
};

// Function to create a new set
set_t* set_new(void) {
    set_t *set = malloc(sizeof(set_t));
    if (set == NULL) {
        return NULL; // Unable to allocate memory for the set
    }
    set->head = NULL;
    return set;
}

// Function to insert a new item with a key into the set
bool set_insert(set_t *set, const char *key, void *item) {
    // Check for NULL set or key
    if (set == NULL || key == NULL || item == NULL) {
        return false;
    }

    // Check if key already exists
    setnode_t *current = set->head;
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return false; // Key already exists in the set
        }
        current = current->next;
    }

    // Create a new node and insert it at the beginning of the list
    setnode_t *new_node = malloc(sizeof(setnode_t));
    if (new_node == NULL) {
        return false; // Unable to allocate memory for the new node
    }

    // Copy the key for use by the set
    new_node->key = strdup(key);
    if (new_node->key == NULL) {
        free(new_node);
        return false; // Unable to allocate memory for the key
    }

    new_node->item = item;
    new_node->next = set->head;
    set->head = new_node;

    return true;
}

// Function to find an item by key in the set
void *set_find(set_t *set, const char *key) {
    // Check for NULL set or key
    if (set == NULL || key == NULL) {
        return NULL;
    }

    // Search for the key in the set
    setnode_t *current = set->head;
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->item; // Key found, return the associated item
        }
        current = current->next;
    }

    return NULL; // Key not found in the set
}

// Function to print the whole set
void set_print(set_t *set, FILE *fp, void (*itemprint)(FILE *fp, const char *key, void *item)) {
    // Check for NULL set
    if (set == NULL) {
        fprintf(fp, "(null)\n");
        return;
    }

    // Iterate over the set and print each (key, item) pair
    setnode_t *current = set->head;
    while (current != NULL) {
        itemprint(fp, current->key, current->item);
        current = current->next;
    }
}

// Function to iterate over all items in the set
void set_iterate(set_t *set, void *arg, void (*itemfunc)(void *arg, const char *key, void *item)) {
    // Check for NULL set or itemfunc
    if (set == NULL || itemfunc == NULL) {
        return;
    }

    // Iterate over the set and call itemfunc on each (key, item) pair
    setnode_t *current = set->head;
    while (current != NULL) {
        itemfunc(arg, current->key, current->item);
        current = current->next;
    }
}

// Function to delete the whole set
void set_delete(set_t *set, void (*itemdelete)(void *item)) {
    // Check for NULL set
    if (set == NULL) {
        return;
    }

    // Iterate over the set, delete each node, and optionally delete each item
    setnode_t *current = set->head;
    while (current != NULL) {
        setnode_t *next = current->next;
        free(current->key);
        if (itemdelete != NULL) {
            itemdelete(current->item);
        }
        free(current);
        current = next;
    }

    // Free the set structure itself
    free(set);
}