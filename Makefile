CXX		:= c++
DEBUG		:= 1
CPPFLAGS	:= -std=c++11 -Wall -Wextra -pedantic -I. -g -DDEBUG=$(DEBUG)

.PHONY: all

all: modules main

clean:
	@echo "Cleaning up ..."
	@rm -rf modules obj main

main: obj/Module.o obj/ModuleManagement.o
	@mkdir -p obj
	@echo "Compiling main.cpp ..."
	@$(CXX) $(CPPFLAGS) main.cpp obj/*.o -o obj/main
	@mv obj/main .

obj/Module.o:
	@mkdir -p obj
	@echo "Compiling src/Module.cpp ..."
	@$(CXX) $(CPPFLAGS) -c src/Module.cpp -o obj/Module.o

obj/ModuleManagement.o: obj/Module.o
	@mkdir -p obj
	@echo "Compiling src/ModuleManagement.cpp ..."
	@$(CXX) $(CPPFLAGS) -c src/ModuleManagement.cpp -o \
	  obj/ModuleManagement.o

modules: modules/Test.so

modules/Test.so:
	@mkdir -p modules
	@echo "Compiling module src/modules/Test.cpp ..."
	@$(CXX) $(CPPFLAGS) -shared src/modules/Test.cpp -o modules/Test.so
