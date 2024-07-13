#include <stddef.h>
#ifndef INPUT_H
#define INPUT_H

extern void print_menu(); // prints the main menu
extern void print_submenu(); // prints the sub menu
extern int get_type(char *); // returns a value associated with a data type (0: int, 1: float, 2: char, 3: string)
extern int is_scientific(char *); // checks if a string contains a single float in scientific notation
extern int is_empty(char *); // checks to see if data contains characters (used to test user input for unnecessary data e.g. "NEW 2" vs "NEW 2 abc")

#endif
