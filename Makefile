#Reference used: https://stackoverflow.com/a/30602701/3791333

CXX := g++
CXXFLAGS := -Wall -Wextra  -std=c++17 -O2
#-Wpedantic
CPPFLAGS := -Iinclude -Ilib/ -MMD -MP -g -fno-stack-protector

EXE ?= ./bin/raytracer
OBJ_DIR ?= build
# OBJ_DIR += build/objects
SRC_DIR ?= src

#VPATH = src/objects

SRC := $(wildcard $(SRC_DIR)/*.cpp) 
SRC += $(wildcard $(SRC_DIR)/objects/*.cpp)
SRC += $(wildcard $(SRC_DIR)/material/*.cpp)

#OBJ := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))
#OBJ = $(SRC:%.cpp=%.o)
# OBJ := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(notdir $(SRC)))
OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))

DEP := $(OBJ:.o=.d)

$(info OBJ: $(OBJ))
$(info DEP: $(DEP))

.PHONY: all clean run #all & clean are not dirs

all: $(EXE) run

$(EXE): $(OBJ)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

run: $(EXE)
	./$(EXE)
	./scripts/show_img.sh
clean:
	@$(RM) -rv $(OBJ_DIR)
-include $(DEP)
