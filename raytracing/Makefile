#Reference used: https://stackoverflow.com/a/30602701/3791333

CXX := g++
CXXFLAGS := -Wall -Wextra -Wpedantic -std=c++17 -O2
CPPFLAGS := -Iinclude -MMD -MP

EXE ?= raytracer
OBJ_DIR ?= ./build
SRC_DIR ?= ./src

SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))

.PHONY: all clean run #all & clean are not dirs

all: $(EXE) run

$(EXE): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@
$(OBJ_DIR):
	mkdir -p $@
run: $(EXE)
	./$(EXE)
	./show_img.sh
clean:
	@$(RM) -rv $(OBJ_DIR)
-include $(OBJ:.o=.d)
