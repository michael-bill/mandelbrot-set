PROJECT_NAME = mandelbrot
SFML_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system
COMPILER = g++
BIN_DIR = ./bin

all: $(PROJECT_NAME)
	$(BIN_DIR)/$(PROJECT_NAME)

$(BIN_DIR)/main.o: main.cpp
	mkdir -p $(BIN_DIR)
	$(COMPILER) -c -o $(BIN_DIR)/main.o main.cpp

$(PROJECT_NAME): $(BIN_DIR)/main.o
	$(COMPILER) $(BIN_DIR)/main.o -o $(BIN_DIR)/$(PROJECT_NAME) $(SFML_FLAGS)

clean:
	rm -rf $(BIN_DIR)/*
