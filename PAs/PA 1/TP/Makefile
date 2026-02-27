CC = gcc
CFLAGS = -Wall -I

SRC_FILES = $(src)/mat.c $(src)/matop.c
OBJ_FILES = $(obj)/mat.o $(obj)/matop.o

HEADERS = $(include)/mat.h $(include)/msgassert.h

EXE = $(bin)/matop

all: $(EXE)
	$(EXE) -s -x 5 -y 5
	$(EXE) -m -x 5 -y 5
	$(EXE) -t -x 5 -y 5

$(EXE): $(OBJ_FILES)
	@mkdir -p $(bin)
	$(CC) $(OBJ_FILES) -o $(EXE)

$(obj)/%.o: $(src)/%.c $(HEADERS)
	@mkdir -p $(obj)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(obj)/*.o $(EXE)

run: all