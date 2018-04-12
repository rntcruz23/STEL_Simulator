CC		= gcc
CFLAGS	= -Wall -lm -g

FILES	= csv
LIB 	= lib
BIN		= bin
SRC		= src

OUTSIM	= outsim
OUTTEST	= outtest
TEST	= test
SIM 	= sim

$(BIN)/$(OUTSIM): $(LIB)/* $(SRC)/$(SIM).c
	mkdir -p $(BIN) $(FILES)
	$(CC) -I$(SRC) -I$(LIB) $(LIB)/*.c $(SRC)/$(SIM).c $(CFLAGS) -o $@
.PHONY: run
run:
	./$(BIN)/$(OUTSIM)

.PHONY: test
$(TEST): $(LIB)/* $(SRC)/$(TEST).c
	mkdir -p $(BIN) $(FILES)
	$(CC) -I$(SRC) -I$(LIB) $(LIB)/*.c $(SRC)/$(TEST).c $(CFLAGS) -o $(BIN)/$(OUTTEST)
.PHONY: run_test
run_test:
	./$(BIN)/$(OUTTEST)

.PHONY: debug
debug:
	gdb ./$(BIN)/$(OUT)
.PHONY: grind
grind:
	valgrind --leak-check=full ./$(BIN)/$(OUT)
.PHONY: clean
clean:
	rm $(BIN)/* $(FILES)/*