TARGET = 5ps

SRCDIR = native/src
INCDIR = native/include
OBJDIR = obj
BINDIR = bin

CC = gcc
CFlags = -Wall -I

LINKER = gcc
LFlags = -Wall -I. -lm

SOURCES := $(wildcard $(SRCDIR)/*.c)
INCLUDES:= $(wildcard $(INCDIR)/*.h)
OBJECTS := $(SOURCES:$(SRCDIR)%.c=$(OBJDIR)/%.o)

$(BINDIR)/$(TARGET): $(OBJECTS)
	@mkdir bin -p
	@$(LINKER) $(OBJECTS) -o $(LFLAGS) $@
	@echo "Linking complete"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@mkdir obj -p
	@$(CC) -c $(CFLAGS) $< -o $@
	@echo "Compiled "$<" successfully"

.PHONY: clean
clean:
	@rm -f $(OBJECTS)
	@rm -f $(BINDIR)/$(TARGET)
