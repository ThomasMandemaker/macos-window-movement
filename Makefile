CC = clang

# Directories
INCLUDE_DIR = include
SOURCE_DIR = src
BIN_DIR = bin
APP_DIR = Schmove.app
CONTENTS_DIR = $(APP_DIR)/Contents
MACOS_DIR = $(CONTENTS_DIR)/MacOS
PLIST_FILE = Info.plist

# Files
INCLUDES = $(wildcard $(INCLUDE_DIR)/*.h)
SOURCES = $(wildcard $(SOURCE_DIR)/*.c)
OBJECTS = $(SOURCES:$(SOURCE_DIR)/%.c=$(BIN_DIR)/%.o)

# Output executable
#TARGET = $(BIN_DIR)/schmove
TARGET = $(MACOS_DIR)/schmove

# Compiler flags
CFLAGS = -I$(INCLUDE_DIR)

# Frameworks
FRAMEWORKS = -framework CoreGraphics -framework ApplicationServices -framework CoreFoundation -framework Carbon

# Linker flags
LDFLAGS = $(FRAMEWORKS)

# Default rule
all: $(APP_DIR)

# Rule to create .app bundle
$(APP_DIR): $(TARGET) $(PLIST_FILE)
	mkdir -p $(MACOS_DIR)
	cp $(BIN_DIR)/schmove $(MACOS_DIR)/
	cp $(PLIST_FILE) $(CONTENTS_DIR)/

# Rule to build the target executable
#$(TARGET): $(OBJECTS)
#    $(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(TARGET): $(OBJECTS)
	mkdir -p $(MACOS_DIR)
	$(CC) $(OBJECTS) -o $(BIN_DIR)/schmove $(LDFLAGS)

# Rule to build object files
#$(BIN_DIR)/%.o: $(SOURCE_DIR)/%.c $(INCLUDES)
#	mkdir -p $(BIN_DIR)
#	$(CC) $(CFLAGS) -c $< -o $@
$(BIN_DIR)/%.o: $(SOURCE_DIR)/%.c $(INCLUDES)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN_DIR)/*.o $(TARGET)

# Phony targets
#.PHONY: all clean
