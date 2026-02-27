# Variáveis
CC = gcc
CFLAGS = -Wall -g -Iinclude -fsanitize=address
LDFLAGS = -fsanitize=address
SRCDIR = src
OBJDIR = obj
BINDIR = bin
EXE = $(BINDIR)/programa

# Arquivos
SRCS = $(SRCDIR)/main.c $(SRCDIR)/escalonador.c $(SRCDIR)/procedimento.c $(SRCDIR)/paciente.c $(SRCDIR)/fila.c
OBJS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))
TARGET = $(EXE)

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

# Reconstrução total
rebuild: clean all
