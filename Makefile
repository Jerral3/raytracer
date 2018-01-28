###
# Variables to set
##################
# Output file name and directory
BIN=raytracer
BUILD_DIR=./build

# C/C++ compiler (gcc/clang recommended)
CC=gcc
CXX=g++

# C/C++ compilation flags
CFLAGS=-std=c99 -g -pedantic -Wall -Wextra -Wshadow -Wpointer-arith \
       -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes \
			 -Wconversion
CXXFLAGS=-std=c++11 -g -Wall -Wextra -pedantic -Wshadow -Weffc++ \
				 -Wconversion -fopenmp -O3

# Linker flags
LDFLAGS=-lgomp

# Source files (adjust if needed)
SRC_CXX=$(wildcard *.cpp)
SRC_C=$(wildcard *.c)

###
# Automatic variables
#####################

# Object files
OBJ_CXX=$(SRC_CXX:%.cpp=$(BUILD_DIR)/%.o)
OBJ_C=$(SRC_C:%.c=$(BUILD_DIR)/%.o)
OBJ=$(OBJ_C)
OBJ+=$(OBJ_CXX)

# Dependencies
DEPS_CXX=$(OBJ_CXX:%.o=%.d)
DEPS_C=$(OBJ_C:%.o=%.d)
DEPS=$(DEPS_C)
DEPS+=$(DEPS_CXX)

###
# Rules
##################

# Main build target
all: check_source_exists $(BIN)

check_source_exists:
ifeq ($(SRC_CXX)$(SRC_C),)
	$(error "No source file. Build failed !")
endif

$(BIN): $(BUILD_DIR)/$(BIN)

# Add this rule to everything that needs
# to be rebuilt if this file changes
FORCE:


# Final linker call
$(BUILD_DIR)/$(BIN): $(OBJ)
	@mkdir -pv $(@D)
	$(CXX) -o $@ $^ $(LDFLAGS)


# Include dependencies
-include $(DEPS)


# General build target
$(BUILD_DIR)/%.o: %.cpp 
	@mkdir -pv $(@D)
	$(CXX) -o $@ -c $< $(CXXFLAGS) -MMD

$(BUILD_DIR)/%.o: %.c 
	@mkdir -pv $(@D)
	$(CC) -o $@ -c $< $(CFLAGS) -MMD


# Clean-up targets
.PHONY: clean mrproper launch debug
clean:
	-rm -f $(BUILD_DIR)/$(BIN) $(OBJ) $(DEPS)

mrproper: clean
	-rm -rf $(BUILD_DIR)

# Launch targets
launch: all
	-$(BUILD_DIR)/$(BIN) $(BIN_ARGS)

debug: all
	-cgdb --quiet --args $(BUILD_DIR)/$(BIN) $(BIN_ARGS)
