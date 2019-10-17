TARGET = 5ps
TARGET_TEST = 5ps_test

SRCDIR = native/src
INCDIR = native/include
OBJDIR = obj
BINDIR = bin

TESTDIR = native/test
LIBDIR = ../libs

CC = gcc
CFlags = -Wall -I

LINKER = gcc
LFlags = -Wall -I. -lm

SOURCES := $(wildcard $(SRCDIR)/*.c)
INCLUDES:= $(wildcard $(INCDIR)/*.h)
TESTS	:= $(wildcard $(TESTDIR)/*.c)
OBJECTS := $(SOURCES:$(SRCDIR)%.c=$(OBJDIR)/%.o)
TESTOBS := $(TESTS:$(TESTDIR)%.c=$(OBJDIR)/%.o)
TESTALLOBS := $(filter-out $(OBJDIR)//main.o, $(TESTOBS) $(OBJECTS))

$(BINDIR)/$(TARGET): $(OBJECTS)
	@mkdir bin -p
	@$(LINKER) $(OBJECTS) -o $(LFLAGS) $@
	@echo "Linking complete"

$(BINDIR)/$(TARGET_TEST): $(TESTALLOBS) $(BINDIR)/$(TARGET)
	@mkdir bin -p
	@$(LINKER) $(TESTALLOBS) -o $(LFLAGS) $@
	@echo "Test Linking complete"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@mkdir obj -p
	@$(CC) -c $(CFLAGS) $< -o $@
	@echo "Compiled "$<" successfully"

$(TESTOBS): $(OBJDIR)/%.o : $(TESTDIR)/%.c
	@mkdir obj -p
	@$(CC) -c $(CFLAGS) $< -o $@ -I $(TESTDIR)
	@echo "Compiled "$<" successfully"

zipup: clean
	@mkdir zip -p
	@mkdir zip/native -p
	@cp native/src/ zip/native -r
	@cp native/include/ zip/native -r
	@cp makefile zip
	@cp README.md zip
	@zip -r out.zip zip


.PHONY: clean
clean:
	@rm -rf $(OBJDIR)
	@rm -rf $(BINDIR)
	@rm -rf zip
	@rm out.zip
