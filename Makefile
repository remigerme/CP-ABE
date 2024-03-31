# -g for debug
# -Wall for warnings
# -Wextra for more warnings
# -O3 for optimization flags
CC = gcc -g -Wall

# list of executables binaries
EXEC = test

# list of libraries to build
LIBS = dgs luca
SPECIFY_LIBS = -L./lib -Wl,-rpath=./lib
LIBS_FLAGS = $(addprefix -l,$(LIBS))
UNIVERSAL_LIBS_FLAGS = -lm -lgmp

default: test

# -lm : link math library
# -lgmp : link gmp library needed for dgs

libs: $(addprefix lib,$(addsuffix .so,$(LIBS)))

# -fPIC : Position Independent Code, needed for building libraries
# -shared : needed for building libraries
# -maes : for aes intrinsics for Luca's algorithmF
lib%.so:lib/%/*.c
	$(CC) -maes -fPIC -shared -o lib/$@ $^

test: matrix.o random.o sampling.o test.c
	$(CC) $(UNIVERSAL_LIBS_FLAGS) $(SPECIFY_LIBS) $(LIBS_FLAGS) -o $@ $^

%.o: %.c
	$(CC) -c -o $@ $^

# We shouldn't need to clean lib subdirectories
clean:
	rm -f $(EXEC) *.o
