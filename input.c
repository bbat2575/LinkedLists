#include "input.h"
#include "list.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// Prints the main menu
void print_menu() {
    printf("\nMAIN MENU:\n");
    printf("- NEW <number of elements> - create a new list\n");
    printf("- INSERT <list index> <element index> <element value> - insert element into a list\n");
    printf("- DELETE <list index> <element index> - delete and element from a list\n");
    printf("- VIEW <list index> - view a specific list by its index\n");
    printf("- VIEW-NESTED <list index> - same as VIEW command but also prints the contents of nested lists\n");
    printf("- VIEW ALL - print the number of lists and each list in order or creation\n");
    printf("- TYPE <list index> - view a specific list by its index, printing out the types of each element\n");
    printf("- REMOVE <list index> - remove a list\n\n");
    printf("Press <CTRL + D> to Quit\n\n");
    printf("> ");
}

// Prints the main menu
void print_submenu() {
    printf("\nType HELP for a list of options\n");
    printf("Press <CTRL + D> to Quit\n\n");
    printf("> ");
}

// Finds the data type of some input
int get_type(char *data) {
    int n_int = 0; // stores the number of integers in the data
    int n_dp = 0; // stores the number of decimal points in the data
    int n_plus = 0; // stores the number of plus signs in the data
    int n_minus = 0; // stores the number of minus signs in the data
    int n_wtspc = 0; // stores the number of whitespace character in the data separately (allows integers & floats to have leading/trailing whitespace)
    int n_char = 0; // stores the number of chars in the data
    int n_curly = 0; // stores number of curly brackets
    int n_leftc = 0; // stores the number of right curly brackets
    type_t type; // stores the data type (type_t defined in mtll.h)

    // Copy the string over (to compare after while loop in is_scientific)
    char *temp = data;

    while(*data) {
        // If integer detected, increment n_int
        if((*data >= 48) & (*data <= 57)) 
            n_int++;
        // If decimal point detected, increment n_dp
        else if(*data == '.')
            n_dp++;
        // If plus sign detected, increment n_plus
        else if(*data == '+')
            n_plus++;
        // If minus sign detected, increment n_minus
        else if(*data == '-')
            n_minus++;
        // If whitespace character detected, increment n_wtspc
        else if(*data == ' ')
            n_wtspc++;
        // If two curly brakets are found with no value in-between OR a third curly bracket is found
        // OR a right curly bracket is detected before any left curly brackets = invalid type
        else if((((*data == '{') & (*(data + 1) == '}'))) | (((*data == '{') | (*(data + 1) == '}')) & (n_curly > 2)) | ((*data == '}') & (n_leftc == 0))) {
            type = invalid;
            return type;
        // If left curly bracket found
        } else if(*data == '{') {
            n_curly++;
            n_leftc++;
            data++;

            // Check for correct list format by checking that *data is a digit until closing right curly bracket is found
            while(*data) {
                // If right bracket is found
                if(*data == '}') {
                    n_curly++;
                    break;
                // If anything else other than a digit is found, type = invalid
                } else if(!isdigit(*data)) {
                    type = invalid;
                    return type;
                }

                data++;
            }
        // If anything else detected (except newline), increment n_char
        } else if(*data != '\n')
            n_char++;

        data++;
    }

    // Compare variable values to derive the data type
    if((n_curly == 2) & (n_int + n_dp + n_plus + n_minus + n_char + n_wtspc == 0)) {
        type = reference;
    // If any curly brackets remain after the while loop check = invalid
    } else if(n_curly > 0) {
        type = invalid;
    } else if((n_int > 0) & (n_dp == 0) & (n_plus + n_minus <= 1) & (n_char == 0))
        type = integer;
    else if(((n_int > 0) & (n_dp == 1) & (n_plus + n_minus <= 1) & (n_char == 0)) | (is_scientific(temp)))
        type = float_point;
    else if((n_int == 0) & (n_dp + n_char + n_plus + n_minus + n_wtspc == 1) & (isprint(*temp) > 0)) // remember to check that character is printable
        type = character;
    else
        type = string;

    return type;
}

// Checks if a string contains a single float in scientific notation
int is_scientific(char *data) {
    char *temp = data;

    int n_e = 0; // stores the number of e and E characters in the data
    int i_e = 0; // gets the index of e or E character (only useful if no more than one exists)
    int n_plus_minus = 0; // stores the number of plus & minus signs in the data
    int count = 0; // used to keep track of indices

    // Count the number of 'e'/'E' characters in data
    while(*data) {
        if((*data == 'e') | (*data == 'E')) {
            n_e++;
            i_e = count;
        // Detect plus signs
        } else if (((*data == '+') | (*data == '-')) & (n_plus_minus < 2))
            n_plus_minus++;
        // If more than two plus or minus signs in total, then it is not in scientific notation
        else if (((*data == '+') | (*data == '-')) & (n_plus_minus == 2))
            return 0;
        // If any invalid characters are detected, then it is not in scientific notation
        else if (!(isdigit(*data)) & (*data != 'e') & (*data != 'E') & (*data != '-') & (*data != '+') & (*data != ' ') & (*data != '\n'))
            return 0;

        data++;
        count++;
    }

    // If there isn't exactly one 'e' or 'E' character OR there's more than two minus signs, data is not in scientific notation
    if(n_e != 1)
        return 0;
    // If a single 'e' or 'E' exists, not in scientific notation
    else if(n_e == count - 1) // account for newline character
        return 0;
    // Check for valid scientific notation format when there are no plus or minus signs
    else if(n_plus_minus == 0) {
        int n_wtspc = 0; // used to store number of whitespace characters in the data

        // Start by checking the left side of the 'e' or 'E' character for valid format of scientific notation
        for(int i = i_e - 1; i >= 0; i--) {
            // Check if an integer is detected, if so, check if any whitespace has occurred already = invalid scientific notation
            if(isdigit(*(temp + i))) {
                if(n_wtspc > 0)
                    return 0;
            } else if (*(temp + i) == ' ')
                n_wtspc++;
        }

        n_wtspc = 0; // reset the value

        // Now check the right side of the 'e' or 'E' character for valid format of scientific notation
        for(int i = i_e + 1; i < count; i++) {
            // Check if an integer is detected, if so, check if any whitespace has occurred already = invalid scientific notation
            if(isdigit(*(temp + i))) {
                if(n_wtspc > 0)
                    return 0;
            } else if (*(temp + i) == ' ')
                n_wtspc++;
        }
    // Check for valid scientific notation format when there's only one plus or minus sign in total
    } else if(n_plus_minus == 1) {
        int n_wtspc = 0; // used to store number of whitespace characters in the data
        n_plus_minus = 0; // reset the value

        // Start by checking the left side of the 'e' or 'E' character for valid format of scientific notation
        for(int i = i_e - 1; i >= 0; i--) {
            // Check if an integer is detected, if so, check if any whitespace or plus/minus signs have occurred already = invalid scientific notation
            if(isdigit(*(temp + i))) {
                if(n_plus_minus + n_wtspc > 0)
                    return 0;
            } else if (((*(temp + i) == '+') | (*(temp + i) == '-'))) {
                if(n_wtspc > 0) // if whitespace has already occurred already = invalid scientific notation
                    return 0;
                else
                    n_plus_minus++;
            } else if (*(temp + i) == ' ')
                n_wtspc++;
        }

        n_wtspc = 0; // reset the value
        int n_int = 0; // used to store number of integers in the data

        // Now check the right side of the 'e' or 'E' character for valid format of scientific notation
        for(int i = i_e + 1; i < count; i++) {
            // Check if plus/minus sign is detected, if so, check if any whitespace or plus/minus signs have occurred already = invalid scientific notation
            if (((*(temp + i) == '+') | (*(temp + i) == '-'))) {
                if(n_int + n_wtspc > 0)
                    return 0;
            } if(isdigit(*(temp + i))) {
                if(n_wtspc > 0) // if whitespace has already occurred already = invalid scientific notation
                    return 0;
                else
                    n_int++;
            } else if (*(temp + i) == ' ')
                n_wtspc++;
        }
    // Check for valid scientific notation format when there are two plus or minus signs in total (it's important to make sure that no more than one occurs on each side of 'e'/'E')
    } else if(n_plus_minus == 2) {
        int n_wtspc = 0; // used to store number of whitespace characters in the data
        n_plus_minus = 0; // reset the value

        // Start by checking the left side of the 'e' or 'E' character for valid format of scientific notation
        for(int i = i_e - 1; i >= 0; i--) {
            // Check if an integer is detected, if so, check if any whitespace or plus/minus signs have occurred already = invalid scientific notation
            if(isdigit(*(temp + i))) {
                if(n_plus_minus + n_wtspc > 0)
                    return 0;
            } else if (((*(temp + i) == '+') | (*(temp + i) == '-'))) {
                if((n_wtspc > 0) | (n_plus_minus == 1)) // if whitespace or plus/minus sign has occurred already = invalid scientific notation
                    return 0;
                else
                    n_plus_minus++;
            } else if (*(temp + i) == ' ')
                n_wtspc++;
        }

        n_wtspc = 0; // reset the value
        n_plus_minus = 0; // reset the value
        int n_int = 0; // used to store number of integers in the data

        // Now check the right side of the 'e' or 'E' character for valid format of scientific notation
        for(int i = i_e + 1; i < count; i++) {
            // Check if a plus/minus sign is detected, if so, check if any integers or whitespace have occurred already = invalid scientific notation
            if (((*(temp + i) == '+') | (*(temp + i) == '-'))) {
                if((n_int + n_wtspc > 0) | (n_plus_minus == 1))
                    return 0;
                n_plus_minus++;
            } if(isdigit(*(temp + i))) {
                if(n_wtspc > 0) // if whitespace has already occurred already = invalid scientific notation
                    return 0;
                else
                    n_int++;
            } else if (*(temp + i) == ' ')
                n_wtspc++;
        }
    } 

    // If all checks passed, then data is in scientific notation
    return 1;
}

// Checks to see if data contains characters (used to test user input for unnecessary data e.g. "NEW 2" vs "NEW 2 abc")
int is_empty(char *data) {
    // Check if pointer has an address (won't have one if no additional input)
    if(data == NULL) {
        return 1;
    }

    // If there's addtional data, make sure it's not a newline character (if not, return 0 -> not empty)
    while(*data) {
        if(*data != '\n') {
            return 0;
        }
        data++;
    }

    // If it was whitespace or newline, return 1 -> empty
    return 1;
}