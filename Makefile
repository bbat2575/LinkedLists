CC=gcc
FLAGS=-Wall -Werror
SOURCE=list.c main.c input.c listmods.c listview.c
OBJS=list.o main.o input.o listmods.o listview.o
TARGET=mtll

# default rule
build: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(FLAGS) $^ -o $@

# run tests
.PHONY:
run_tests: tests
	./test.sh

# clean compiled binary/object files
.PHONY:
clean:
	rm -f $(OBJS)
	rm -f $(TARGET)
