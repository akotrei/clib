CURRENT_DIR = $(shell pwd)

# set optimization flag
CXX_FLAG = O3
ifneq ($(flag),)
CXX_FLAG := $(flag)
endif

include 	= $(CURRENT_DIR)/include
src			= $(CURRENT_DIR)/src
lib			= $(CURRENT_DIR)/lib

####################################
# container
container_c 	= $(src)/container.c
container_o 	= container.o
container_so 	= $(lib)/libcontainer.so

container_obj: $(container_c)
	gcc -Wall -$(CXX_FLAG) -fPIC \
	-I $(include) \
	-c $(container_c) \
	-o $(container_o)

container_so: container_obj
	gcc -Wall -$(CXX_FLAG) -shared \
	-o $(container_so) $(container_o)
	rm $(container_o)
####################################

####################################
# list
list_c 	= $(src)/list.c
list_o 	= list.o
list_so 	= $(lib)/liblist.so

list_obj: $(list_c)
	gcc -Wall -$(CXX_FLAG) -fPIC \
	-I $(include) \
	-c $(list_c) \
	-o $(list_o)

list_so: list_obj
	gcc -Wall -$(CXX_FLAG) -shared \
	-o $(list_so) $(list_o)
	rm $(list_o)
####################################

####################################
# test
test_c = $(CURRENT_DIR)/test/test.c
test_o = $(CURRENT_DIR)/test/test.o
test_exe = $(CURRENT_DIR)/test/test

test_obj: $(test_c)
	gcc -Wall -$(CXX_FLAG) \
	-I $(include) \
	-c $(test_c) \
	-o $(test_o)

test_exe: test_obj
	gcc -Wall -$(CXX_FLAG) \
	-o $(test_exe) $(test_o) \
	-L$(lib) -lcontainer
	rm $(test_o)
# ========================
####################################