#include "input.h"
#include "list.h"
#include "listmods.h"
#include "listview.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Create a node for a linked list
struct node *node_create(struct list *lists) {
    struct node *temp; // Used to store the node

    temp = (struct node *)malloc(sizeof(struct node));

    // Get user input for node value
    char value[129]; // size = 129 bytes to make room for terminating null byte
    if(fgets(value, 129, stdin) == NULL) { // if EOF entered, free temp and return NULL
        free(temp);
        return NULL;
    }

    type_t type = get_type(value);
    
    // Assign value to the node based on its type
    if(type == integer)
        temp->i = atoi(value);
    else if(type == float_point)
        temp->f = atof(value);
    else if(type == character)
        temp->c = value[0];
    else if (type == string) {
        // Strip the newlines character from the string before assignment
        for(int i = 0; i < 129; i++) {         
            if(value[i] == '\n') {
                value[i] = '\0';
                break;
            }
        }
        strcpy(temp->s, value);
    } else if (type == reference) {
        // Check if the list referenced in curly braces exists
        struct list *referenced = list_exists(lists, value);

        // If list found and is not a nested list, set the node's value to point to it
        if(referenced != NULL) {
            if(!referenced->is_nested) {
                referenced->is_referenced++;
                temp->lst = referenced;
            }
        // Else set to NULL to indicate invalid
        } else
            temp->lst = NULL;
    }

    temp->type = type; // node data type

    return temp;
}

// Frees all the nodes in a linked list
void list_free(struct node *head) {
   struct node *current_node; // Used to store each node before freeing it
   struct node *next_node = head; // Used to store the next node

    // Iterate through the nodes and free them
    while (next_node) {
        current_node = next_node;
        next_node = next_node->next;
        free(current_node);
    }
}

// Frees all linked lists that exist
void list_free_all(struct list *lists) {
   struct list *temp;

   while(lists) {
        temp = lists; // set the current node
        if(temp->head != NULL)
            list_free(temp->head); // free the linked list that this node points to
        lists = lists->next; // point to the next linked list
        free(temp); // free the current node
    }
}

// Removes a linked list (note: to actually modify the first list in lists (index 0), you need a ptr to a ptr)
int list_remove(struct list **lists_ptr, int index, struct list *lists) {
    struct list *temp = lists;
    struct list *prev; // used to store the previous list that was checked
    int count = 0; // used to keep track of number of nodes traversed (useful since due to prior node removals, any index could be the first node)

    while(temp) {
        // Check if the current list has the index we're looking for
        if(temp->index == index) {
            // If the list is refernced in another list, don't remove it!
            if(temp->is_referenced)
                break;
            // If it's not the first list
            if(count > 0) {
                // Change the previous list's next value to temp's next value (remove temp from the linked list pointer sequence)
                if(temp->next != NULL) {
                    prev->next = temp->next;
                }
                else {
                    prev->next = NULL;
                }
            // If it's the first list
            } else
                *lists_ptr = temp->next;

            // If the list is nested, find any references and decrement those list's is_referenced values to indicate one less reference
            if(temp->is_nested) {
                struct node *node = temp->head;
                while(node) {
                    // If the node contains a reference
                    if(node->type == reference)
                        node->lst->is_referenced--;
                    node = node->next;
                }
            }

            // Free the list
            list_free(temp->head);
            free(temp);

            // Return 1 to indicate success
            return 1;
        }

        prev = temp; // set temp to prev
        temp = temp->next; // change temp to the next list
        count++;
    }

    // Return 0 to indicate that no list with that index was found or the list was irremovable (since is referenced in another list)
    return 0;
}