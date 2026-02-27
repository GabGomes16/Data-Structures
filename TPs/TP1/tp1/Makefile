# Variáveis
CC = gcc
CFLAGS = -Wall -g -Iinclude -fsanitize=address
LDFLAGS = -fsanitize=address
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# Arquivos
SRCS = $(SRCDIR)/main.c $(SRCDIR)/ordInd.c $(SRCDIR)/sorts.c
OBJS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))
TARGET = $(BINDIR)/tp1.out

# Regra padrão
all: $(TARGET)

# Criação do executável
$(TARGET): $(OBJS)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compilação de cada objeto
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# Limpeza
clean:
	rm -rf $(OBJDIR) $(BINDIR)

rebuild: clean all
