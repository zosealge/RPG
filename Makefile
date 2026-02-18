# Output binary
BIN = rpg

# Put all auto generated stuff to this build dir.
BUILD_DIR = .

SOURCE_DIR = src

# List of all the source files.
SOURCE = \
	main.c \

INCLUDES = \
	src \

CC = gcc

CC_FLAGS = -g -Wall -Wextra -Werror
LNK_FLAGS = -lncurses

# All .o files go to build dir.
C_OBJ = $(SOURCE:%.c=$(BUILD_DIR)/%.o)
OBJ = $(C_OBJ)


# Gcc/Clang will create these .d files containing dependencies.
DEP = $(OBJ:%.o=%.d)
# Include paths with a -I in front of them
INCLUDEPATHS = $(INCLUDES:%= -I %)

# Actual target of the binary - depends on all .o files.
$(BIN) : PreBuild $(OBJ)
	-mkdir -p $(@D) 2>/dev/null
	$(CC) $(OBJ) $(LNK_FLAGS) -o $(BIN)

# Include all .d files
-include $(DEP)

# Build target for every single object file.
# The potential dependency on header files is covered
# by calling `-include $(DEP)`.

$(BUILD_DIR)/%.o : $(SOURCE_DIR)/%.c
	# Compiling RPG
#-mkdir -p $(@D) 2>/dev/null
	$(CC) $(CC_FLAGS) $(INCLUDEPATHS) -MMD -c $< -o $@


.PHONY : clean
clean:
#-rm -r $(BUILD_DIR)
	-rm $ main.d 
	-rm $ main.o

.PHONY: PreBuild
PreBuild:
	# This is run at the start of the build