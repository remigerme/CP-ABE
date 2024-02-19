# -g for debug
# -Wall for warnings
# -Wextra for more warnings
CC = gcc -g -Wall

# list of executables binaries
EXEC = test

# For cleaning purposes
SUBDIRS = . dgs
SUBDIRSCLEAN = $(addsuffix clean,$(SUBDIRS))

default: test

# -lm : link math library
# -lgmp : link gmp library needed for dgs

random.o: dgs/dgs_bern.o dgs/dgs_gauss_dp.o dgs/dgs_rround_dp.o random.c
	$(CC) -lm -lgmp -c $^ -o $@

test: dgs/dgs_bern.o dgs/dgs_gauss_dp.o dgs/dgs_rround_dp.o random.c test.c
	$(CC) -lm -lgmp -o $@ $^

%.o: %.c
	$(CC) -c -o $@ $^

# Cleaning subdirs
clean: $(SUBDIRSCLEAN)

clean_curdir:
	rm -f $(EXEC) *.o

%clean: %
	$(MAKE) -C $< -f $(PWD)/Makefile clean_curdir
