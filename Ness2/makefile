TARGET=schedule
TARGET_CHILD=prime
TEST=$(TARGET)_test

MainSrcDir = native/src/main
ChildSrcDir = native/src/child
TestSrcDir = native/test

RootObjDir = obj
MainObjDir = $(RootObjDir)
ChildObjDir = $(RootObjDir)
TestObjDir = $(RootObjDir)/test

BinDir=bin
GcovDir=gcov

CC=gcc
CFlags = -Wall -Werror
LFlags =


MainSources := $(wildcard $(MainSrcDir)/*.c)
ChildSources := $(wildcard $(ChildSrcDir)/*.c)
TestSources := $(wildcard $(TestSrcDir)/*.c)

MainObjs := $(MainSources:$(MainSrcDir)%.c=$(MainObjDir)%.o)
ChildObjs := $(ChildSources:$(ChildSrcDir)%.c=$(ChildObjDir)%.o)
TestObjs := $(TestSources:$(TestSrcDir)%.c=$(TestObjDir)%.o)

build: $(MainObjs) child
	@mkdir $(BinDir) -p
	@$(CC) $(MainObjs) -o $(BinDir)/$(TARGET) $(LFlags)
	@echo "Linking "$(TARGET)" complete"

child: $(ChildObjs)
	@mkdir $(BinDir) -p
	@$(CC) $(ChildObjs) -o $(BinDir)/$(TARGET_CHILD) $(LFlags)
	@echo "Linking "$(TARGET_CHILD)" complete"

test: $(TestObjs)
	@mkdir $(GcovDir) -p
	@make build BinDir=$(GcovDir) MainObjDir=$(GcovDir) ChildObjDir=$(GcovDir) CFlags=--coverage LFlags=-fprofile-generate
	@$(CC) $(TestObjs) -o $(LFlags) $(GcovDir)/$(TEST)
	@cp $(MainSrcDir)/* $(GcovDir)
	@cp $(ChildSrcDir)/* $(GcovDir)
	@echo "Linking "$(TEST)" complete"

$(MainObjs): $(MainObjDir)/%.o : $(MainSrcDir)/%.c
	@mkdir $(MainObjDir) -p
	@$(CC) -c $(CFlags) $< -o $@
	@echo "Compiled "$<" successfully"

$(ChildObjs): $(ChildObjDir)/%.o : $(ChildSrcDir)/%.c
	@mkdir $(ChildObjDir) -p
	@$(CC) -c $(CFlags) $< -o $@
	@echo "Compiled "$<" successfully"

$(TestObjs): $(TestObjDir)/%.o : $(TestSrcDir)/%.c
	@mkdir $(TestObjDir) -p
	@$(CC) -c $(CFlags) $< -o $@
	@echo "Compiled "$<" successfully"

.PHONY: clean
clean:
	@rm -rf $(RootObjDir)
	@rm -rf $(BinDir)
	@rm -rf $(GcovDir)
	@rm -f *.xml
	@rm -rf zip
	@rm -f demo.txt
	@rm -f Ness2.zip

zipup: clean
	@make
	@./demomake.sh
	@mkdir zip -p
	@mv demo.txt zip/
	@cp -r native zip/
	@rm -r zip/native/test
	@cp makefile zip/
	@cp README.md zip/
	@cp test.txt zip/
	@cp test2.txt zip/
	@zip -r tmp.zip zip
	@make clean
	@rm -rf zip
	@mv tmp.zip Ness2.zip

