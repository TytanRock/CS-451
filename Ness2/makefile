TARGET = schedule
TARGET_CHILD = prime
TARGET_TEST = schedule_test

ROOTOBJDIR = obj

SRCDIR = native/src/main
CHILDDIR = native/src/child
INCDIR = native/include
OBJDIR = obj/main
CHILDOBJDIR = obj/child
BINDIR = bin
GCOVDIR = gcov

TESTDIR = native/test
LIBDIR = ../libs

CC = gcc
CFlags = -Wall -I

LINKER = gcc
LFlags = -Wall -I. -lm

SOURCES := $(wildcard $(SRCDIR)/*.c)
CHILDSOURCES := $(wildcard $(CHILDDIR)/*.c)
INCLUDES:= $(wildcard $(INCDIR)/*.h)
OBJECTS := $(SOURCES:$(SRCDIR)%.c=$(OBJDIR)/%.o)
CHILDOBS := $(CHILDSOURCES:$(CHILDDIR)%.c=$(CHILDOBJDIR)/%.o)

TESTOBS := $(TESTS:$(TESTDIR)%.c=$(OBJDIR)/%.o)
TESTS	:= $(wildcard $(TESTDIR)/*.c)
GCOVOBS := $(SOURCES:$(SRCDIR)%.c=$(GCOVDIR)/%.o)
TESTALLOBS := $(filter-out $(OBJDIR)//main.o, $(TESTOBS) $(OBJECTS))

$(BINDIR)/$(TARGET): $(OBJECTS) $(BINDIR)/$(TARGET_CHILD)
	@mkdir $(BINDIR) -p
	@$(LINKER) $(OBJECTS) -o $(LFLAGS) $@
	@echo "Linking "$< " complete"

$(BINDIR)/$(TARGET_CHILD): $(CHILDOBS)
	@mkdir $(BINDIR) -p
	@$(LINKER) $(CHILDOBS) -o $(LFLAGS) $@
	@echo "Linking "$<" complete"

$(GCOVDIR)/$(TARGET_TEST): $(TESTALLOBS) $(GCOVDIR)/$(TARGET)
	@mkdir $(GCOVDIR) -p
	@$(LINKER) $(TESTALLOBS) -o $(LFLAGS) $@
	@echo "Test Linking complete"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@mkdir $(OBJDIR) -p
	@$(CC) -c $(CFLAGS) $< -o $@
	@echo "Compiled "$<" successfully"

$(CHILDOBS): $(CHILDOBJDIR)/%.o : $(CHILDDIR)/%.c
	@mkdir $(CHILDOBJDIR) -p
	@$(CC) -c $(CFLAGS) $< -o $@
	@echo "Compiled "$<" successfully"

$(TESTOBS): $(OBJDIR)/%.o : $(TESTDIR)/%.c
	@mkdir $(OBJDIR) -p
	@$(CC) -c $(CFLAGS) $< -o $@ -I $(TESTDIR)
	@echo "Compiled "$<" successfully"

$(GCOVOBS): $(GCOVDIR)/%.o : $(SRCDIR)/%.c
	@mkdir $(GCOVDIR) -p
	@$(CC) -c $(CFLAGS) --coverage $< -o $@
	@echo "compiled "$<" successfully"
	@cp $< $(GCOVDIR)

$(GCOVDIR)/$(TARGET): $(GCOVOBS)
	@mkdir $(GCOVDIR) -p
	@$(CC) $(GCOVOBS) --coverage -o $@
	@echo gcov complete

zipup: clean
	@mkdir zip -p
	@mkdir zip/native -p
	@cp native/src/ zip/native -r
	@cp native/include/ zip/native -r
	@cp makefile zip
	@cp README.md zip
	@make
	@./demomake.sh
	@cp demo.txt zip
	@zip -r tmp.zip zip
	@make clean
	@mv tmp.zip out.zip


.PHONY: clean
clean:
	@rm -rf $(ROOTOBJDIR)
	@rm -rf $(BINDIR)
	@rm -rf zip
	@rm -f out.zip
	@rm -f *.xml
	@rm -f demo.txt
	@rm -rf $(GCOVDIR)
