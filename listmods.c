#include "input.h"
#include "list.h"
#include "listmods.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Inserts a node at the specified index of a list
int insert(struct list *lists, struct node **head_ptr, struct node *head, int index, char *data) {
    // It's important to check if a referenced list exists before proceeding (since there's no reason to continue if it doesn't)
    struct list *referenced; // Used to store a referenced list
    type_t type = get_type(data); // Get data's data type

    // If the new node's data type is a referenced list, check that the referenced list exists
    if(type == reference) {
        referenced = list_exists(lists, data);
        // If the list doesn't exist or it's a nested list, return 0 (abide by depth limitation) -> need 2 separate conditionals or we get a seg fault
        if(referenced == NULL)
            return 0;
        else if (referenced->is_nested)
            return 0;
    }

    struct node *current = head; // Store the current node
    struct node *prev = NULL; // Store the previous node
    int size = 0; // Used to store the total size of the list
    int current_index = 0; // Used to keep track of node indices while traversing list
    int node_found = 0; // used to indicate if the requested node (at index) is found

    // Get the size of the list
    while(current) {
        current = current->next;
        size++;
    }

    // If the index provided is negative, convert it to the positive index version
    if(index < 0) {
        index = size + index + 1; // compute the index required
    }

    current = head; // reset current back to the first node

    // If list is empty and index other than 0 is given, return 0 (this includes -1, c.f. index is made positive above)
    if((head == NULL) & (index != 0))
        return 0;
    // If list is empty and the correct index is given, increment node_found to allow processing below
    else if((head == NULL) & (index == 0))
        node_found++;
    // Else if list is not empty, search for the node at the given index
    else {
        // Iterate through each node to find the node with the index we're looking for
        while(current) {
            // If node @ index is found
            if(current_index == index) {
                node_found++;
                break;
            } else
                current_index++;
            prev = current;
            current = current->next;
        }
    }

    // If the list has been iterated through and node not found, check if user is asking for a new node on the end of the list
    if((!node_found) & (index == current_index)) {
        node_found++;
    }

    // If index found, update previous node's next value and remove the current node
    if(node_found) {
        // If list is empty
        if(head == NULL) {
            *head_ptr = (struct node *)malloc(sizeof(struct node)); // create the new node and the head value for this list
            prev = *head_ptr; // change prev to the new node
            prev->next = NULL; // update the new node's next value to NULL
        // If the first node
        } else if(prev == NULL) {
            *head_ptr = (struct node *)malloc(sizeof(struct node)); // create the new node and the head value for this list
            prev = *head_ptr; // change prev to the new node
            prev->next = current; // update the new node's next value to the current node
        // If adding a new node onto the end
        } else if (current == NULL) {
            prev->next = (struct node *)malloc(sizeof(struct node)); // create the new node and update the previous node's next value
            prev = prev->next; // change prev to the new node
            prev->next = NULL; // update the new node's next value to NULL
        // For any other case
        } else {
            prev->next = (struct node *)malloc(sizeof(struct node)); // create the new node and update the previous node's next value
            prev = prev->next; // change prev to the new node
            prev->next = current; // update the new node's next value to the current node
        }
    
        // Assign value to the new node based on its type
        if(type == integer)
            prev->i = atoi(data);
        else if(type == float_point)
            prev->f = atof(data);
        else if(type == character)
            prev->c = data[0];
        else if (type == string)
            strcpy(prev->s, data);
        else if (type == reference) {
            referenced->is_referenced++;
            prev->lst = referenced;
        }

        // Set the new node's data type value
        prev->type = type; // node data type

        // Return 1 to indicate success
        return 1;
    // Else return 0 to indicate that the index does not exist
    } else
        return 0;
}

// Deletes the node at the specified index of a list
int delete(struct list *linked_list, struct node **head_ptr, struct node *head, int index) {
    struct node *current = head; // Store the current node
    struct node *prev = NULL; // Store the previous node
    int size = 0; // Used to store the total size of the list
    int current_index = 0; // Used to keep track of node indices while traversing list
    int node_found = 0; // used to indicate if the requested node (at index) is found

    // If list is empty, return 0 to indicate node doesn't exist
    if(head == NULL)
        return 0;

    // If the index provided is negative, get the size of the list (used to count from the end of the list)
    if(index < 0) {
        while(current) {
            current = current->next;
            size++;
        }
        index = size + index; // compute the index required
    }

    current = head; // reset current back to the first node
    
    // Iterate through each node to find the right index
    while(current) {
        // If node @ index is found
        if(current_index == index) {
            node_found++;
            break;
        } else
            current_index++;
        prev = current;
        current = current->next;
    }

    // If index found, update previous node's next value and remove the current node
    if(node_found) {
        // If the node being removed is a list refernced, then indicate this in the referenced list
        type_t node_type = current->type;
        if(node_type == reference)
            current->lst->is_referenced--;

        // If the first node
        if(prev == NULL)
            *head_ptr = current->next;
        // If not the first node
        else
            prev->next = current->next;

        // Free the node struct
        free(current);

        // After removal of a reference node, evaluate if the list is still nested
        if(node_type == reference) {
            current = *head_ptr; // reset current back to the first node

            int is_nested = 0;
            // Iterate through each node to find the right index
            while(current) {
                // If this node is reference type
                if(current->type == reference) {
                    is_nested = 1;
                    break;
                }
                current = current->next;
            }

            linked_list->is_nested = is_nested;
        }

        // If *head_ptr is NULL return 2 to indicate deletion resulted in an empty list
        if(*head_ptr == NULL)
            return 2;
        // Else simply return 1 to indicate node was deleted
        else
            return 1;
    // Else return 0 to indicate that the index does not exist
    } else
        return 0;
}

// Checks a list for any invalid types
int check_invalid(struct node *head) {
    struct node *node = head; // Used to store each node starting the head
    int is_invalid = 0; // Used to indicate that invalid type was found

    // Iterate through each node and check for invalid types
    while(node) {
        if(node->type == invalid) {
            is_invalid++;
            break;
        }

        node = node->next;
    }

    // If no invalid types where found, return 0
    if(!is_invalid)
        return 0;
    else {
        node = head; // Reset node back to the head

        // Iterate over the list and decrement the is_referenced value for any referenced lists
        while(node) {
            if(node->type == reference)
                node->lst->is_referenced--;

            node = node->next;
        }

        // Return 1 to indicate invalid type was found
        return 1;
    }
}

// Checks if a certain referenced list exists
struct list *list_exists(struct list *lists, char *data) {
    char *data_ptr = data;

    // Remove the left curly brackets from input (for conversion to int below)
    *data_ptr = ' ';

    // Check if the list referenced in curly braces exists
    int list_found = 0;
    
    while(lists) {
        if((lists->index) == atoi(data)) {
            list_found++;
            break;
        }
        lists = lists->next;
    }

    // List found
    if(list_found)
        return lists;
    // List not found
    else {
        return NULL;
    }
}