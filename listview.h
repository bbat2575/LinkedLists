#include "list.h"
#include <stddef.h>
#ifndef LISTVIEW_H
#define LISTVIEW_H

extern void list_view(struct node *); // prints the given linked list to stdout
extern void list_view_nested(struct node *); // prints the given linked list to stdout and also prints referenced lists
extern void list_view_all(struct list *); // prints all linked lists to stdout
extern void list_type(struct node *); // prints the linked list's data types to stdout

#endif