#include "input.h"
#include "list.h"
#include "listmods.h"
#include "listview.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFLEN (129)

int main(int argc, char** argv) {
    int index = 0; // Keeps track of each list's index
    struct list *lists = NULL; // A linked list that holds pointers to each linked list created
    char buffer[BUFLEN]; // An array used to store user input

    print_submenu();

    // Continuously take user input until EOF issued
    while(fgets(buffer, BUFLEN, stdin)) {
        // Split the input at whitespace delimiter and store the command
        char *token = strtok(buffer, " ");

        // If NEW command issued and has no leading whitespace
        if(((strcmp(token, "NEW") == 0) | (strcmp(token, "NEW\n") == 0)) & (buffer[0] != ' ')) {
            // If command is missing arguments
            if(strcmp(token, "NEW\n") == 0) {
                printf("INVALID COMMAND: NEW\n");
                printf("\n> ");
                continue;
            }

            char *token2 = strtok(NULL, " "); // Get argument
            char *next = strtok(NULL, " "); // Get anything else on the line

            // Check if argument has plus/minus sign
            if((token2[0] == '+') | (token2[0] == '-')) {
                printf("INVALID COMMAND: NEW\n");
                printf("\n> ");
                continue;
            }

            // Checks if command argument is an integer, that no additional input follows it, and that there's only a single space between NEW and the argument
            if((get_type(token2) == integer) & (is_empty(next)) & (buffer[4] != ' ')) {
                int num = atoi(token2); // Convert argument to integer
                struct list *temp = lists; // Stores the current list being worked on (from struct list *lists)
                struct list *prev = NULL; // Stores the previous list of temp

                // Create a new pointer to a list in lists
                // If this is the first list to be created:
                if(lists == NULL) {
                    // Create a node to point to the new list
                    lists = (struct list *)malloc(sizeof(struct list));
                    lists->index = index;
                    index++;
                    temp = lists;
                // If not the first list to be created:
                } else {
                    // Find the node in lists that points to the last linked list created
                    while(temp->next) {
                        temp = temp->next;
                    }
                    // Create a node to point to the new list
                    temp->next = (struct list *)malloc(sizeof(struct list));
                    // Set temp to this next node
                    prev = temp;
                    temp = temp->next;
                    temp->index = index;
                    index++;
                }

                // Set the new list's is_referenced and is_nested values to 0
                temp->is_referenced = 0;
                temp->is_nested = 0;
                // Set the new list's next value to NULL (end of the list)
                temp->next = NULL;

                if(num > 0) {
                    struct node *temp_node; // Used to hold nodes of the new linked list

                    int eof_entered = 0; // Used to indicate if user inputs EOF while creating linked list nodes

                    // Create the new linked list
                    for(int i = 0; i < num; i++) {
                        // Create the head of a new list
                        if(i == 0) {
                            temp->head = node_create(lists);
                            temp_node = temp->head;
                            // If EOF entered during node creation, increment eof_entered and exit for loop
                            if(temp_node == NULL) {
                                eof_entered++;
                                break;
                            }
                            temp_node->next = NULL;
                        // Create remaining node (after head) of a new list
                        } else {
                            temp_node->next = node_create(lists);
                            temp_node = temp_node->next;
                            // If EOF entered during node creation, increment eof_entered and exit for loop
                            if(temp_node == NULL) {
                                eof_entered++;
                                break;
                            }
                            temp_node->next = NULL;
                        }                
                        
                        // If new node is a list reference, set is_nested to 1
                        if((temp_node->type == reference) & (temp_node->lst != NULL))
                            temp->is_nested = 1;
                        // If new node fails assigning list referece, set node type to invalid for list removal (below)
                        else if((temp_node->type == reference) & (temp_node->lst == NULL))
                            temp_node->type = invalid;
                    }

                    // If EOF entered during node creation, stop taking user input and exit
                    if(eof_entered)
                        break;

                    // If any invalid types were inputted into the new list, remove it
                    if(check_invalid(temp->head)) {
                        printf("INVALID COMMAND: NEW\n");
                        list_free(temp->head);
                        free(temp);
                        // Decrement the index
                        index--;
                        // If it was the only list that existed
                        if(prev == NULL)
                            lists = NULL;
                        // If there are other lists
                        else
                            prev->next = NULL;
                    } else if (temp->is_nested) {
                        // Print the new nested list's contents
                        printf("Nested %d: ", temp->index);
                        list_view(temp->head);
                    } else {
                        // Print the new simple list's contents
                        printf("List %d: ", temp->index);
                        list_view(temp->head);
                    }
                // Indicate that this list is empty
                } else {
                    temp->head = NULL;
                    // Print the new list's contents
                    printf("List %d: \n", temp->index);
                }
            // Invalid argument for NEW command
            } else
                printf("INVALID COMMAND: NEW\n");

        // If VIEW command issued and has no leading whitespace
        } else if(((strcmp(token, "VIEW") == 0) | (strcmp(token, "VIEW\n") == 0)) & (buffer[0] != ' ')) { 
            // If command is missing arguments
            if(strcmp(token, "VIEW\n") == 0) {
                printf("INVALID COMMAND: VIEW\n");
                printf("\n> ");
                continue;
            }

            char *token2 = strtok(NULL, " "); // Get argument
            char *next = strtok(NULL, " "); // Get anything else on the line

            // Check if argument has plus/minus sign
            if((token2[0] == '+') | (token2[0] == '-')) {
                printf("INVALID COMMAND: VIEW\n");
                printf("\n> ");
                continue;
            }

            // Checks if command argument is an integer, that no additional input follows it, and that there's only a single space between VIEW and argument
            if((get_type(token2) == integer) & (is_empty(next)) & (buffer[5] != ' ')) {
                int num = atoi(token2); // Convert argument to integer

                struct list *temp = lists;

                // If no lists exist for viewing
                if(temp == NULL)
                    printf("INVALID COMMAND: VIEW\n");
                else {
                    while(temp) {
                        if(temp->index == num) {
                            list_view(temp->head);
                            break;
                        }
                        
                        // If the index was not found (NULL reached = end of lists)
                        if(temp->next == NULL)
                            printf("INVALID COMMAND: VIEW\n");

                        temp = temp->next;
                    }
                }
            // If command argument is the word ALL (+ \n = user hits enter) and that there's only a single space between VIEW and ALL
            } else if((strcmp(token2, "ALL\n") == 0) & (buffer[5] != ' '))
                list_view_all(lists);
            // Invalid argument for VIEW command
            else
                printf("INVALID COMMAND: VIEW\n");
        // If VIEW-NESTED command issued and has no leading whitespace
        } else if(((strcmp(token, "VIEW-NESTED") == 0) | (strcmp(token, "VIEW-NESTED\n") == 0)) & (buffer[0] != ' ')) { 
            // If command is missing arguments
            if(strcmp(token, "VIEW-NESTED\n") == 0) {
                printf("INVALID COMMAND: VIEW-NESTED\n");
                printf("\n> ");
                continue;
            }

            char *token2 = strtok(NULL, " "); // Get argument
            char *next = strtok(NULL, " "); // Get anything else on the line

            // Check if argument has plus/minus sign
            if((token2[0] == '+') | (token2[0] == '-')) {
                printf("INVALID COMMAND: VIEW-NESTED\n");
                printf("\n> ");
                continue;
            }

            // Checks if command argument is an integer, that no additional input follows it, and that there's only a single space between VIEW and argument
            if((get_type(token2) == integer) & (is_empty(next)) & (buffer[5] != ' ')) {
                int num = atoi(token2); // Convert argument to integer

                struct list *temp = lists;

                // If no lists exist for viewing
                if(temp == NULL)
                    printf("INVALID COMMAND: VIEW-NESTED\n");
                else {
                    while(temp) {
                        if(temp->index == num) {
                            list_view_nested(temp->head);
                            break;
                        }
                        
                        // If the index was not found (NULL reached = end of lists)
                        if(temp->next == NULL)
                            printf("INVALID COMMAND: VIEW-NESTED\n");

                        temp = temp->next;
                    }
                }
            // Invalid argument for VIEW-NESTED command
            } else
                printf("INVALID COMMAND: VIEW-NESTED\n");
        // If TYPE command issued and has no leading whitespace
        } else if(((strcmp(token, "TYPE") == 0) | (strcmp(token, "TYPE\n") == 0)) & (buffer[0] != ' ')) {
            // If command is missing arguments
            if(strcmp(token, "TYPE\n") == 0) {
                printf("INVALID COMMAND: TYPE\n");
                printf("\n> ");
                continue;
            }

            char *token2 = strtok(NULL, " "); // Get argument
            char *next = strtok(NULL, " "); // Get anything else on the line

            // Check if argument has plus/minus sign
            if((token2[0] == '+') | (token2[0] == '-')) {
                printf("INVALID COMMAND: TYPE\n");
                printf("\n> ");
                continue;
            }

            // Checks if argument is an integer, that no additional input follows it, and that there's only a single space between TYPE and argument
            if((get_type(token2) == integer) & (is_empty(next)) & (buffer[5] != ' ')) {
                int num = atoi(token2);

                struct list *temp = lists;

                // If no lists exist for type viewing
                if(temp == NULL)
                    printf("INVALID COMMAND: TYPE\n");
                else {
                    while(temp) {
                        if(temp->index == num) {
                            list_type(temp->head);
                            break;
                        }
                        
                        // If the index was not found (NULL reached = end of lists)
                        if(temp->next == NULL)
                            printf("INVALID COMMAND: TYPE\n");

                        temp = temp->next;
                    }
                }
            // Invalid argument for TYPE command
            } else
                printf("INVALID COMMAND: TYPE\n");
        // If REMOVE command issued and has no leading whitespace
        } else if(((strcmp(token, "REMOVE") == 0) | (strcmp(token, "REMOVE\n") == 0)) & (buffer[0] != ' ')) {
            // If command is missing arguments
            if(strcmp(token, "REMOVE\n") == 0) {
                printf("INVALID COMMAND: REMOVE\n");
                printf("\n> ");
                continue;
            }

            char *token2 = strtok(NULL, " "); // Get argument
            char *next = strtok(NULL, " "); // Get anything else on the line

            // Check if argument has plus/minus sign
            if((token2[0] == '+') | (token2[0] == '-')) {
                printf("INVALID COMMAND: REMOVE\n");
                printf("\n> ");
                continue;
            }

            // Checks if argument is an integer, that no additional input follows it, and that there's only a single space between REMOVE and argument
            if((get_type(token2) == integer) & (is_empty(next)) & (buffer[7] != ' ')) {
                int num = atoi(token2);

                // Attempt to remove the list. If successful, print out all the lists that exist
                if(list_remove(&lists, num, lists)) {
                    printf("List %d has been removed.\n\n", num);
                    list_view_all(lists);
                // If remove fails
                } else
                    printf("INVALID COMMAND: REMOVE\n");
            // Invalid argument for REMOVE command
            } else
                printf("INVALID COMMAND: REMOVE\n");
            // If INSERT command issued and has no leading whitespace
        } else if(((strcmp(token, "INSERT") == 0) | (strcmp(token, "INSERT\n") == 0)) & (buffer[0] != ' ')) {
            // If command is missing arguments
            if(strcmp(token, "INSERT\n") == 0) {
                printf("INVALID COMMAND: INSERT\n");
                printf("\n> ");
                continue;
            }

            char *token2 = strtok(NULL, " "); // Get the list index
            char *token3 = strtok(NULL, " "); // Get the node index
            char *token4 = strtok(NULL, "\n"); // Get the new node's value all the way up to (but not including) the newline character
            char *trailing_space; // Used to test if the 2nd argument ends with a single trailing whitespace and no other values afterwards

            // Check if list id has plus/minus sign
            if((token2[0] == '+') | (token2[0] == '-')) {
                printf("INVALID COMMAND: INSERT\n");
                printf("\n> ");
                continue;
            }

            // If 2nd argument is available
            if(token3 != NULL)
                trailing_space = strchr(token3, '\n');
            // If token3 is NULL then the 2nd argument is missing
            else {
                printf("INVALID COMMAND: INSERT\n");
                printf("\n> ");
                continue;
            }

            // If there's no 3rd argument provided and 2nd argument doesn't have any trailing whitespace
            if((token4 == NULL) & (trailing_space != NULL)) {
                printf("INVALID COMMAND: INSERT\n");
                printf("\n> ");
                continue;
            // If there's no 3rd argument provided and argument 2 has a single trailing whitespace
            } else if ((token4 == NULL) & (trailing_space == NULL))
                token4 = "";

            type_t data_type = get_type(token4); // Store the data type

            // Checks if the arguments are integers, new node value is not invalid (token3), that no additional input follows it, and that there's only a single space between INSERT and arguments
            if((get_type(token2) == integer) & (get_type(token3) == integer) & (data_type != invalid) & (buffer[7] != ' ')) {
                int num = atoi(token2); // Stores list id
                int num2 = atoi(token3); // Stores node index

                // If no lists exist
                if(lists == NULL) {
                    printf("INVALID COMMAND: INSERT\n");
                } else {
                    struct list *temp = lists;
                    int list_found = 0; // Used to indicate if the requested list (at index == num) is found

                    // Find the list with the right id
                    while(temp) {    
                        if(temp->index == num) {
                            list_found++;
                            break;
                        } else
                            temp = temp->next;
                    }
                    
                    // If the list is found
                    if(list_found) {
                        // If the node to be added will contain a reference type, check that the list is not already referenced else where (abide by depth limitation)
                        if((data_type == reference) & (temp->is_referenced)) {
                            printf("INVALID COMMAND: INSERT\n");
                            printf("\n> ");
                            continue;
                        }

                        // Attempt to insert the node at index = num2 from the list (with node value = token4). If successful, print out the list
                        int successful = insert(lists, &(temp->head), temp->head, num2, token4);

                        // If a reference is added to this list for the first time, update it's is_nested value
                        if(successful & (data_type == reference) & !temp->is_nested)
                            temp->is_nested = 1;
                        
                        // If insertion was succesful
                        if(successful) {
                            if(temp->is_nested)
                                printf("Nested %d: ", temp->index);
                            else
                                printf("List %d: ", temp->index);

                            list_view(temp->head);
                        // If insertion failed
                        } else
                            printf("INVALID COMMAND: INSERT\n");
                    } else {
                        printf("INVALID COMMAND: INSERT\n");
                    }
                }
            // Invalid argument for REMOVE command
            } else
                printf("INVALID COMMAND: INSERT\n");
        // If DELETE command issued and has no leading whitespace
        } else if(((strcmp(token, "DELETE") == 0) | (strcmp(token, "DELETE\n") == 0)) & (buffer[0] != ' ')) {
            // If command is missing arguments
            if(strcmp(token, "DELETE\n") == 0) {
                printf("INVALID COMMAND: DELETE\n");
                printf("\n> ");
                continue;
            }

            char *token2 = strtok(NULL, " "); // Get next value on the line
            char *token3 = strtok(NULL, " "); // Get the final value on the line
            char *next = strtok(NULL, " "); // Get anything else on the line

            // Check if list id has plus/minus sign
            if((token2[0] == '+') | (token2[0] == '-')) {
                printf("INVALID COMMAND: DELETE\n");
                printf("\n> ");
                continue;
            }
            
            // If the 2nd argument is available
            if(token3 != NULL) {
                // Test if the 2nd argument ends with trailing whitespace
                char *trailing_space = strchr(token3, '\n');
                if(trailing_space == NULL) {
                    printf("INVALID COMMAND: DELETE\n");
                    printf("\n> ");
                    continue;
                }
            // If token3 is NULL then the 2nd argument is missing
            } else {
                printf("INVALID COMMAND: DELETE\n");
                printf("\n> ");
                continue;
            }

            // Checks if the arguments are integers, that no additional input follows it, and that there's only a single space between DELETE and arguments
            if((get_type(token2) == integer) & (get_type(token3) == integer) & (is_empty(next)) & (buffer[7] != ' ')) {
                int num = atoi(token2); // Stores list id
                int num2 = atoi(token3); // Stores node index

                // If no lists exist
                if(lists == NULL) {
                    printf("INVALID COMMAND: DELETE\n");
                } else {
                    struct list *temp = lists;
                    int list_found = 0; // Used to indicate if the requested list (at index == num) is found

                    // Find the list with the right id
                    while(temp) {         
                        if(temp->index == num) {
                            list_found++;
                            break;
                        } else               
                            temp = temp->next;
                    }
                    
                    if(list_found) {
                        // Attempt to delete the node at index = num2 from the list. If successful, print out the list
                        int result = delete(temp, &(temp->head), temp->head, num2);
                        
                        // If deletion was succesful
                        if(result == 1) {
                            printf("List %d: ", temp->index);
                            list_view(temp->head);
                        // If deletion resulted in an empty list
                        } else if (result == 2)
                            printf("List %d: \n", temp->index);
                        // If deletion failed
                        else
                            printf("INVALID COMMAND: DELETE\n");
                    } else {
                        printf("INVALID COMMAND: DELETE\n");
                    }
                }
            // Invalid argument for REMOVE command
            } else
                printf("INVALID COMMAND: DELETE\n");
        // Else the input is invalid
        } else if(((strcmp(token, "HELP") == 0) | (strcmp(token, "HELP\n") == 0)) & (buffer[0] != ' ')) {
            print_menu();
            continue;
        } else {
            printf("INVALID COMMAND: INPUT\n");
        }

        printf("\n> ");
    }
    printf("\n");

    // Upon EOF, deallocate all lists from memory
    list_free_all(lists);
     
    return 0;
}