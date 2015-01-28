CDEBUG		:= 0
CPPFLAGS	:= -std=c++11 -Wall -Wextra -pedantic -I. -DDEBUG=$(CDEBUG)
CXX		:= c++
CLEANO		:= $(shell find . -type f -name "*.o" | sed 's/^\.\///')
CLEANSO		:= $(shell find . -type f -name "*.so" | sed 's/^\.\///')
MODULES		:= $(shell find src/modules -type f -name "*.cpp")
MODULES		:= $(shell echo $(MODULES) | sed 's/cpp$$/so/')

.PHONY:		all clean modules

all:		main modules

clean:
	@$(foreach item,$(CLEANO),echo $(item); rm -f $(item);)
	@$(foreach item,$(CLEANSO),echo $(item); rm -f $(item);)

main:		src/ModuleManagement.o main.o
	@echo " [LNK] $@ ..."
	@$(CXX) $(CPPFLAGS) -o main $^

modules:	$(MODULES)

%.so:		%.cpp
	@echo " [CXX] $@ ..."
	@$(CXX) $(CPPFLAGS) -shared -o $@ $^

%.o:		%.cpp
	@echo " [CXX] $@ ..."
	@$(CXX) $(CPPFLAGS) -c -o $@ $^
