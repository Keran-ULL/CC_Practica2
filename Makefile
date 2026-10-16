CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -I./include

BUILD_DIR = build

SOURCES = $(shell find src -name "*.cc")
OBJ = $(patsubst src/%.cc,$(BUILD_DIR)/%.o,$(SOURCES))

MAIN_FILE = $(shell grep -l "int main" src/*.cc src/*/*.cc 2>/dev/null | head -1)
EXECUTABLE = $(BUILD_DIR)/$(basename $(notdir $(MAIN_FILE)))

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: src/%.cc
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean run

clean:
	rm -rf $(BUILD_DIR)

run: $(EXECUTABLE)
	./$(EXECUTABLE)