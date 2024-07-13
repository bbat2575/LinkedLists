#include "input.h"
#include "list.h"
#include "listmods.h"
#include "listview.h"
#include <stdio.h>

// View the contents of a list
void list_view(struct node *head) {
    struct node *node = head; // Used to store each node starting with the head

    // Iterate through each node's value and based on the data type, print the value
    while(node) {   
        // If integer
        if(node->type == integer)
            printf("%d", node->i);
        // If float
        else if(node->type == float_point)
            printf("%.2f", node->f);
        // If character
        else if(node->type == character)
            printf("%c", node->c);
        // If strings
        else if(node->type == string)
            printf("%s", node->s);
        else if(node->type == reference)
            printf("{List %d}", node->lst->index);

        node = node->next;

        if(node == NULL)
            printf("\n");
        else
            printf(" -> ");
    }
}

// View the contents of a list
void list_view_nested(struct node *head) {
    struct node *node = head; // Used to store each node starting with the head

    // Iterate through each node's value and based on the data type, print the value
    while(node) {   
        // If integer
        if(node->type == integer)
            printf("%d", node->i);
        // If float
        else if(node->type == float_point)
            printf("%.2f", node->f);
        // If character
        else if(node->type == character)
            printf("%c", node->c);
        // If string
        else if(node->type == string)
            printf("%s", node->s);
        else if(node->type == reference) {
            struct node *node2 = node->lst->head; // Used to store each node starting with the head

            printf("{"); // Print opening curly bracket

            // Iterate through each node's value and based on the data type, print the value
            while(node2) {   
                // If integer
                if(node2->type == integer)
                    printf("%d", node2->i);
                // If float
                else if(node2->type == float_point)
                    printf("%.2f", node2->f);
                // If character
                else if(node2->type == character)
                    printf("%c", node2->c);
                // If string
                else if(node2->type == string)
                    printf("%s", node2->s);
                else if(node2->type == reference)
                    printf("{List %d}", node2->lst->index);

                node2 = node2->next;

                if(node2 != NULL)
                    printf(" -> ");
            }

            printf("}"); // Print closing curly bracket
        }

        node = node->next;

        if(node == NULL)
            printf("\n");
        else
            printf(" -> ");
    }
}

void list_view_all(struct list *lists) {
    struct list *temp = lists; // start with the first list
    int total = 0;

    // Count the total number of lists
    while(temp) { 
        temp = temp->next; // set temp to the next list 
        total++;
    }

    printf("Number of lists: %d\n", total);
    temp = lists;

    while(temp) {
        // Print nested list
        if(temp->is_nested)
            printf("Nested %d\n", temp->index); // print the list and its index
        // Print simple list
        else
            printf("List %d\n", temp->index); // print the list and its index
        temp = temp->next; // set temp to the next list 
    }
}

// View the data types for the contents of a list
void list_type(struct node *head) {
    struct node *node = head; // Used to store each node starting with the head

    // Iterate through each node's value and based on the data type, print the value
    while(node) {   
        // If integer
        if(node->type == integer)
            printf("int");
        // If float
        else if(node->type == float_point)
            printf("float");
        // If character
        else if(node->type == character)
            printf("char");
        // If string
        else if(node->type == string)
            printf("string");
        else if(node->type == reference)
            printf("reference");

        node = node->next;

        if(node == NULL)
            printf("\n");
        else
            printf(" -> ");
    }
}