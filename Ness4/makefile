TARGET=memory
#TARGET_CHILD=prime
TEST=$(TARGET)_test

MainSrcDir = native/src/
#ChildSrcDir = native/src/child
TestSrcDir = native/test

RootObjDir = obj
MainObjDir = $(RootObjDir)
#ChildObjDir = $(RootObjDir)
TestObjDir = $(RootObjDir)/test

BinDir=bin
GcovDir=gcov

CC=gcc
CFlags = -Wall -Werror
LFlags = -pthread


MainSources := $(wildcard $(MainSrcDir)/*.c)
#ChildSources := $(wildcard $(ChildSrcDir)/*.c)
TestSources := $(wildcard $(TestSrcDir)/*.c)

MainObjs := $(MainSources:$(MainSrcDir)%.c=$(MainObjDir)%.o)
#ChildObjs := $(ChildSources:$(ChildSrcDir)%.c=$(ChildObjDir)%.o)
TestObjs := $(TestSources:$(TestSrcDir)%.c=$(TestObjDir)%.o)

build: $(MainObjs)
	@mkdir $(BinDir) -p
	@$(CC) $(MainObjs) -o $(BinDir)/$(TARGET) $(LFlags)
	@echo "Linking "$(TARGET)" complete"

#child: $(ChildObjs)
#	@mkdir $(BinDir) -p
#	@$(CC) $(ChildObjs) -o $(BinDir)/$(TARGET_CHILD) $(LFlags)
#	@echo "Linking "$(TARGET_CHILD)" complete"

test: $(TestObjs)
	@mkdir $(GcovDir) -p
	@make build BinDir=$(GcovDir) MainObjDir=$(GcovDir) CFlags=--coverage LFlags=-fprofile-generate\ -pthread
	@$(CC) $(TestObjs) -o $(GcovDir)/$(TEST) $(LFlags) -pthread
	@echo $(MainSrcDir)
	@cp $(MainSrcDir)/* $(GcovDir)
	@echo "Linking "$(TEST)" complete"

$(MainObjs): $(MainObjDir)/%.o : $(MainSrcDir)/%.c
	@mkdir $(MainObjDir) -p
	@$(CC) -c $(CFlags) $< -o $@
	@echo "Compiled "$<" successfully"

#$(ChildObjs): $(ChildObjDir)/%.o : $(ChildSrcDir)/%.c
#	@mkdir $(ChildObjDir) -p
#	@$(CC) -c $(CFlags) $< -o $@
#	@echo "Compiled "$<" successfully"

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
	@rm -rf Ness3
	@rm -f demo.txt
	@rm -f Ness3.zip

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
	@cp longrun.txt zip/
	@mv zip Ness3
	@zip -r tmp.zip Ness3
	@make clean
	@rm -rf Ness3
	@mv tmp.zip Ness3.zip

