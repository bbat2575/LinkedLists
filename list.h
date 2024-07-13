#include <stddef.h>
#ifndef LIST_H
#define LIST_H

// The data types for linked list nodes
typedef enum {
    integer,
    float_point,
    character,
    string,
    reference, // used for reference types (pointers to lists)
    invalid // used for invalid values (e.g. {})
} type_t;

// A single node of a linked list
struct node {
    type_t type;
    union {
        int i;
        float f;
        char c;
        char s[129]; // last element reserved for terminating null byte
        struct list *lst;
    };
    struct node *next;
};

// A node that refers to the head of a linked list
struct list {
    int index;
    int is_nested; // set to 1 if the list is a nested list, set to 0 if a simple list
    int is_referenced; // indicates the number of times this list is referenced in other lists
    struct node *head; // the head of a list
    struct list *next; // the next head of the next list
};

extern struct node *node_create(struct list *); // creates a new linked list (NEW command)
extern void list_free(); // deallocate a linked list from heap
extern void list_free_all(); // deallocate all linked lists from heap
extern int list_remove(struct list **, int, struct list *); // removes a node/struct from a linked list

#endif
