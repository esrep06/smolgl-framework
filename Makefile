UNAME_S:=$(shell uname -s)
CC=g++
CFLAGS=-std=c++20 
INCLUDE_DIR=include
GLFW_LIB_DIR=$(INCLUDE_DIR)/glfw/build/src/
GLEW_LIB_DIR=$(INCLUDE_DIR)/glew/lib/
BUILD_DIR=build
LDFLAGS=-O0 -Wall -Wextra -pedantic -lpthread -ldl -lm $(GLEW_LIB_DIR)libGLEW.a $(GLFW_LIB_DIR)libglfw3.a

#List of directories containing source files
SRC_DIRS=src/engine

# Create a list of source files in each directory
SRC+=src/main.cpp
SRC+=$(shell find $(SRC_DIRS) -type f -name '*.cpp')

EXECUTABLE=$(BUILD_DIR)/app

ifeq ($(UNAME_S),Darwin)
    LDFLAGS += -framework OpenGL -framework CoreVideo -framework Cocoa -framework IOKit -framework QuartzCore 
else 
	LDFLAGS += -lGL
endif

$(EXECUTABLE): $(SRC)
	$(CC) $(CFLAGS)  $^ -o $@ -g -I$(INCLUDE_DIR) $(LDFLAGS)  

.PHONY: clean
clean:
	rm -f $(EXECUTABLE)

