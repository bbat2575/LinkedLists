#include "list.h"
#include <stddef.h>
#ifndef LISTMODS_H
#define LISTMODS_H

extern int insert(struct list *, struct node **, struct node *, int, char *); // inserts a node into a linked list
extern int delete(struct list *, struct node **, struct node *, int); // deletes a node from a linked list
extern int check_invalid(struct node *); // checks a list for invalid types
extern struct list *list_exists(struct list *, char *); // checks if a certain referenced list exists

#endif
