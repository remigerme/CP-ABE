# -g for debug
# -Wall for warnings
# -Wextra for more warnings
CC = gcc -g -Wall

# list of executables binaries
EXEC = test

# list of libraries to build
LIBS = dgs luca
SPECIFY_LIBS = -L./lib -Wl,-rpath=./lib
LIBS_FLAGS = $(addprefix -l,$(LIBS))
UNIVERSAL_LIBS_FLAGS = -lm -lgmp

# For cleaning purposes
SUBDIRS = . lib $(addprefix lib/,$(LIBS))
SUBDIRSCLEAN = $(addsuffix clean,$(SUBDIRS))

default: test

# -lm : link math library
# -lgmp : link gmp library needed for dgs

libs: $(addprefix lib,$(addsuffix .so,$(LIBS)))

# -fPIC : Position Independent Code, needed for building libraries
# -shared : needed for building libraries
# -maes : for aes intrinsics
lib%.so:lib/%/*.c
	$(CC) -maes -fPIC -shared -o lib/$@ $^

test: random.o sampling.o test.c
	$(CC) $(UNIVERSAL_LIBS_FLAGS) $(SPECIFY_LIBS) $(LIBS_FLAGS) -o $@ $^

%.o: %.c
	$(CC) -O3 -march=native -mtune=native -fomit-frame-pointer -ftree-vectorize -funsafe-math-optimizations -mfpmath=sse -mavx2 -ftree-vectorize  -maes -c -o $@ $^

# Cleaning subdirs
clean: $(SUBDIRSCLEAN)

clean_curdir:
	rm -f $(EXEC) *.o

%clean: %
	$(MAKE) -C $< -f $(PWD)/Makefile clean_curdir
