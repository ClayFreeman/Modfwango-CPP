CDEBUG		:= 0
CPPFLAGS	:= -std=c++11 -Wall -Wextra -pedantic -I. -DDEBUG=$(CDEBUG)
CXX		:= c++
CLEANO		:= $(shell find . -type f -name "*.o" | sed 's/^\.\///')
CLEANSO		:= $(shell find . -type f -name "*.so" | sed 's/^\.\///')
DEPEND		:= $(shell find src -maxdepth 1 -type f -name "*.cpp")
DEPEND		:= $(shell echo $(DEPEND) | sed 's/cpp$$/o/')
MODULES		:= $(shell find src/modules -maxdepth 1 -type f -name "*.cpp")
MODULES		:= $(shell echo $(MODULES) | sed 's/cpp$$/so/')

.PHONY:		all clean modules
all:		main modules
modules:	$(MODULES)

clean:
	@$(foreach item,$(CLEANO),echo $(item); rm -f $(item);)
	@$(foreach item,$(CLEANSO),echo $(item); rm -f $(item);)

main:		$(DEPEND) main.o
	@echo " [LNK] $@ ..."
	@$(CXX) $(CPPFLAGS) -o main -ldl $^

%.so:		%.cpp
	@echo " [CXX] $@ ..."
	@$(CXX) $(CPPFLAGS) -shared -o $@ $^

%.o:		%.cpp
	@echo " [CXX] $@ ..."
	@$(CXX) $(CPPFLAGS) -c -o $@ $^
