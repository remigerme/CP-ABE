CC = gcc
CFLAGS = -g -Wall -O3
INCLUDES = -Isrc -Isrc/bgg -Isrc/sampling -Isrc/utils

SRC_DIR = src
BUILD_DIR = build
SRC_TEST = tests

OBJS_RAW = common matrix attribute random sampling circuit gen_circuit bgg cp
OBJS_O = $(addsuffix .o,$(OBJS_RAW))
OBJS = $(addprefix $(BUILD_DIR)/,$(OBJS_O))

# list of executables binaries
EXEC_RAW = sampling circuit bgg cp_bit gen_circuit is_short cp
EXEC = $(addprefix test_,$(EXEC_RAW))

# build cp as a library, including math lib
SPECIFY_LIBS = -L./build '-Wl,-rpath,./build'
LIBS_FLAGS = -lm $(SPECIFY_LIBS) -lcp


default: libcp.so

tests: $(EXEC)


# Building shared library
libcp.so: $(OBJS)
	$(CC) -shared -o $(BUILD_DIR)/$@ $^

# Brute-force searching
# Using -fPIC as the files will be included in shared lib
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -fPIC -c -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/utils/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -fPIC -c -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/bgg/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -fPIC -c -o $@ $^

# -maes required for random.c using aes intrinsics
$(BUILD_DIR)/%.o: $(SRC_DIR)/sampling/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -maes -fPIC -c -o $@ $^


# Rule for building tests
test_%: $(SRC_TEST)/test_%.c
	$(CC) $(CFLAGS) $(LIBS_FLAGS) $(INCLUDES) -o $(BUILD_DIR)/$@ $^


# We clean the lib too
clean:
	rm -f $(BUILD_DIR)/*.o $(BUILD_DIR)/*.so $(addprefix $(BUILD_DIR)/,$(EXEC))

.PHONY: tests clean
