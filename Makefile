# -g for debug
CC = gcc -g

# list of executables binaries
EXEC = program

default: program

# Optionnal flags
# -Wall : enables warning
# -lm : link math library

program: main.o utils.o
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -c -o $@ $^

clean:
	rm -f $(EXEC) *.o