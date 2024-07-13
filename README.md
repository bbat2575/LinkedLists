# Description

A multi-type linked list data structure program.

Properties:
* Linked lists are dynamic - elements can be inserted and removed.
* Lists can be nested to a maximum depth of 1.
* If a list is referenced inside another list, it cannot be removed.

The program contains the following functionalities:
1. NEW [number of elements] - create a new list
1. INSERT [list index] [element index] [element value] - insert element into a list
1. DELETE [list index] [element index] - delete and element from a list
1. VIEW [list index] - view a specific list by its index
1. VIEW-NESTED [list index] - same as VIEW command but also prints the contents of nested lists
1. VIEW ALL - print the number of lists and each list in order or creation
1. TYPE [list index] - view a specific list by its index, printing out the types of each element
1. REMOVE [list index] - remove a list
1. Press <CTRL + D> to Quit

# How To Run:

Compile binary and execute:

```bash
make
./mtll
```

# Examples:

**NEW** - When a new list is created, subsequent inputs will populate the list:

```bash
> NEW 4
32
6.4
a
hello
List 0: 32 -> 6.40 -> a -> hello

> VIEW 0
32 -> 6.40 -> a -> hello

> TYPE 0
int -> float -> char -> string
```

**NEW w/ Nested**:

```bash
> NEW 2
1
2
List 0: 1 -> 2

> NEW 3
{0}
3
4
Nested 1: {List 0} -> 3 -> 4

> VIEW-NESTED 1
{1 -> 2} -> 3 -> 4

> TYPE 1
reference -> int -> int

> VIEW ALL
Number of lists: 2
List 0
Nested 1

> REMOVE 0
INVALID COMMAND: REMOVE

> REMOVE 1
List 1 has been removed.

Number of lists: 1
List 0

> REMOVE 0
List 0 has been removed.

Number of lists: 0
```

**INSERT & DELETE**:

```bash
> NEW 2
0
2
List 0: 0 -> 2

> INSERT 0 1 1
List 0: 0 -> 1 -> 2

> INSERT 0 -1 3
List 0: 0 -> 1 -> 2 -> 3

> DELETE 0 0
List 0: 1 -> 2 -> 3
```

# How To Run Tests

Simply execute the testing script.

```bash
./test.sh
```

NOTE: Ensure execution privileges are available.

```bash
chmod +x test.sh
```