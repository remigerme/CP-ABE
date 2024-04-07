# -g for debug
# -Wall for warnings
# -Wextra for more warnings
# -O2 or -O3 for optimization flags
CC = gcc
CFLAGS = -g -Wall -O3
INCLUDES = -Isrc -Isrc/bgg -Isrc/sampling -Isrc/utils -Ilib/dgs

SRC_DIR = src
BUILD_DIR = build
SRC_TEST = src/test

OBJS_RAW = matrix attribute random sampling circuit bgg cp
OBJS_O = $(addsuffix .o,$(OBJS_RAW))
OBJS = $(addprefix $(BUILD_DIR)/,$(OBJS_O))

# list of executables binaries
EXEC = test_sampling test_circuit test_bgg test_cp

# list of libraries to build
LIBS = dgs
SPECIFY_LIBS = -L./build '-Wl,-rpath,$$ORIGIN'
LIBS_FLAGS = $(SPECIFY_LIBS) $(addprefix -l,$(LIBS))
# -lm : link math library
# -lgmp : link gmp library needed for dgs
UNIVERSAL_LIBS_FLAGS = -lm -lgmp

.PHONY: tests libs clean

tests: $(EXEC)

libs: $(addprefix lib,$(addsuffix .so,$(LIBS)))

# -fPIC : Position Independent Code, needed for building libraries
# -shared : needed for building libraries
# -maes : for aes intrinsics for Luca's algorithmF
lib%.so:lib/%/*.c
	$(CC) -maes -fPIC -shared -o $(BUILD_DIR)/$@ $^


# Rule for building tests
test_%: $(OBJS) $(SRC_TEST)/test_%.c
	$(CC) $(CFLAGS) $(UNIVERSAL_LIBS_FLAGS) $(LIBS_FLAGS) $(INCLUDES) -o $(BUILD_DIR)/$@ $^


# Brute-force searching
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/utils/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/bgg/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/sampling/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $^


# We shouldn't need to clean lib subdirectories
clean:
	rm -f $(BUILD_DIR)/*.o $(addprefix $(BUILD_DIR)/,$(EXEC))
