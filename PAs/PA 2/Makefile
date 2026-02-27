CC = gcc
LIBS = -lm
SRC = src
OBJ = obj
INC = include
BIN = bin
OBJS = $(OBJ)/pa1.o 
HDRS = 
CFLAGS = -Wall -c -g -I$(INC)

EXE = $(BIN)/pa1.out

all: $(EXE)

$(EXE): $(OBJS) | $(BIN)
	$(CC) -o $(EXE) $(OBJS) $(LIBS)

$(OBJ)/pa1.o: $(HDRS) pa1.c | $(OBJ)
	$(CC) $(CFLAGS) -o $(OBJ)/pa1.o pa1.c

$(BIN):
	mkdir -p $(BIN)

$(OBJ):
	mkdir -p $(OBJ)

clean:
	rm -f $(BIN)/*.out $(EXE) $(OBJS) gmon.out