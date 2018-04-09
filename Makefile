CC		= gcc
CFLAGS	= -Wall -lm -g

OUT		= sim
FILES	= csv
LIB 	= lib
BIN		= bin
SRC		= src

$(BIN)/$(OUT): $(LIB)/* $(SRC)/*
	$(CC) -I$(SRC) -I$(LIB) $(LIB)/*.c $(SRC)/*.c $(CFLAGS) -o $@
.PHONY: debug
debug:
	gdb ./$(BIN)/$(OUT)
.PHONY: grind
grind:
	valgrind --leak-check=full ./$(BIN)/$(OUT)
.PHONY: run
run:
	./$(BIN)/$(OUT)
.PHONY: clean
clean:
	rm $(BIN)/* $(FILES)/*