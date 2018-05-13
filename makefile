# Generic Build Details
CXX = g++
OPTS = 	-std=c++14 -g -Wall -Wextra -Wshadow -Wnon-virtual-dtor \
				-Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual
LINKFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lGL -lGLU -lGLEW -lIPCommonUtils
TEST_LINKFLAGS = -lcppunit



# Details of builds

SRC_EXE_NAME = Pong.exe
TEST_EXE_NAME = TestPong.exe

EXE_DIR = bin
OBJ_DIR = obj
SRC_DIR = src
TEST_DIR = test

SRC_EXE = $(EXE_DIR)/$(SRC_EXE_NAME)
TEST_EXE = $(EXE_DIR)/$(TEST_EXE_NAME)

# Files

# Source files

SRC_FILES := $(shell find $(SRC_DIR) -name '*.cpp')
TEST_FILES := $(shell find $(TEST_DIR) -name '*.cpp')

# Object files

SRCS := $(SRC_FILES:.cpp=.o)
TESTS := $(TEST_FILES:.cpp=.o)

SRC_OBJ_FILES := $(patsubst %, $(OBJ_DIR)/%, $(SRCS))
TEST_OBJ_FILES := $(patsubst %, $(OBJ_DIR)/%, $(TESTS))



# Link

# src
$(SRC_EXE): $(SRC_OBJ_FILES)
	$(CXX) -o $@ $^ $(LINKFLAGS)

# test
$(TEST_EXE): $(TEST_OBJ_FILES)
	$(CXX) -o $@ $^ $(TEST_LINKFLAGS)


# Compile

# src
$(SRC_OBJ_FILES): $(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(OPTS) $(LINKFLAGS) -c -o $@ $<

# test
$(TEST_OBJ_FILES): $(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(OPTS) $(LINKFLAGS) -c -o $@ $<


# Commands
.PHONY: clean-prototype clean-all clean-src clean-test build-all build-src build-test build-prototype
.default: build

clean-src:
	rm -f $(SRC_OBJ_FILES) $(SRC_EXE)

clean-test:
	rm -f $(TEST_OBJ_FILES) $(TEST_EXE)

build-src: $(SRC_EXE)

build-test: $(TEST_EXE)

build-prototype: $(PROTOTYPE_EXE)

clean: clean-src

build: build-src

clean-all: clean-src clean-test

build-all: build-src build-test
