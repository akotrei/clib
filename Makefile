CURRENT_DIR = $(shell pwd)
DESTDIR?=/usr/local/lib

# set optimization flag
CXX_FLAG = O3
ifneq ($(flag),)
CXX_FLAG := $(flag)
endif

include 	= $(CURRENT_DIR)/include
src			= $(CURRENT_DIR)/src
lib			= $(CURRENT_DIR)/lib

####################################
# make install
install:
	cp -n $(lib)/* ${DESTDIR}

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
# tree
tree_c 	= $(src)/tree.c
tree_o 	= tree.o
tree_so = $(lib)/libtree.so

tree_obj: $(tree_c)
	gcc -Wall -$(CXX_FLAG) -fPIC \
	-I $(include) \
	-I $(include)/tree \
	-c $(tree_c) \
	-o $(tree_o)

tree_so: tree_obj
	gcc -Wall -$(CXX_FLAG) -shared \
	-o $(tree_so) $(tree_o)
	rm $(tree_o)
####################################

####################################
# allocator_std
allocator_std_c 	= $(src)/allocator_std.c
allocator_std_o 	= allocator_std.o
allocator_std_so 	= $(lib)/liballocator_std.so

allocator_std_obj: $(allocator_std_c)
	gcc -Wall -$(CXX_FLAG) -fPIC \
	-I $(include) \
	-c $(allocator_std_c) \
	-o $(allocator_std_o)

allocator_std_so: allocator_std_obj
	gcc -Wall -$(CXX_FLAG) -shared \
	-o $(allocator_std_so) $(allocator_std_o)
	rm $(allocator_std_o)
####################################

####################################
# tree_iterator
tree_iterator_c 	= $(src)/tree_iterator.c
tree_iterator_o 	= tree_iterator.o
tree_iterator_so 	= $(lib)/libtree_iterator.so

tree_iterator_obj: $(tree_iterator_c)
	gcc -Wall -$(CXX_FLAG) -fPIC \
	-I $(include) \
	-I $(include)/tree \
	-c $(tree_iterator_c) \
	-o $(tree_iterator_o)

tree_iterator_so: tree_iterator_obj
	gcc -Wall -$(CXX_FLAG) -shared \
	-o $(tree_iterator_so) $(tree_iterator_o)
	rm $(tree_iterator_o)
####################################

####################################
# algorithm
algorithm_c 	= $(src)/algorithm.c
algorithm_o 	= algorithm.o
algorithm_so 	= $(lib)/libalgorithm.so

algorithm_obj: $(algorithm_c)
	gcc -Wall -$(CXX_FLAG) -fPIC \
	-I $(include) \
	-c $(algorithm_c) \
	-o $(algorithm_o)

algorithm_so: algorithm_obj
	gcc -Wall -$(CXX_FLAG) -shared \
	-o $(algorithm_so) $(algorithm_o)
	rm $(algorithm_o)
####################################

####################################
# list
list_c 	= $(src)/list.c
list_o 	= list.o
list_so = $(lib)/liblist.so

list_obj: $(list_c)
	gcc -Wall -$(CXX_FLAG) -fPIC \
	-I $(include) \
	-I $(include)/list \
	-c $(list_c) \
	-o $(list_o)

list_so: list_obj
	gcc -Wall -$(CXX_FLAG) -shared \
	-o $(list_so) $(list_o)
	rm $(list_o)
####################################

####################################
<<<<<<< HEAD
# array
array_c  = $(src)/array.c
array_o  = array.o
array_so = $(lib)/libarray.so

array_obj: $(array_c)
	gcc -Wall -$(CXX_FLAG) -fPIC \
	-I $(include) \
	-I $(include)/array \
	-c $(array_c) \
	-o $(array_o)

array_so: array_obj
	gcc -Wall -$(CXX_FLAG) -shared \
	-o $(array_so) $(array_o)
	rm $(array_o)
####################################

####################################
# hash_table
hash_table_c  = $(src)/hash_table.c
hash_table_o  = hash_table.o
hash_table_so = $(lib)/libhash_table.so

hash_table_obj: $(hash_table_c)
	gcc -Wall -$(CXX_FLAG) -fPIC \
	-I $(include) \
	-I $(include)/hash_table \
	-I $(include)/list \
	-I $(include)/array \
	-c $(hash_table_c) \
	-o $(hash_table_o)

hash_table_so: hash_table_obj
	gcc -Wall -$(CXX_FLAG) -shared \
	-o $(hash_table_so) $(hash_table_o)
	rm $(hash_table_o)
=======
# test_tree
test_tree_c = $(CURRENT_DIR)/test/test_tree.c
test_tree_o = $(CURRENT_DIR)/test/test_tree.o
test_tree_exe = $(CURRENT_DIR)/test/test_tree

test_tree_obj: $(test_tree_c)
	gcc -Wall -$(CXX_FLAG) \
	-I $(include) \
	-I $(include)/tree \
	-c $(test_tree_c) \
	-o $(test_tree_o)

test_tree_exe: test_tree_obj
	gcc -Wall -$(CXX_FLAG) \
	-o $(test_tree_exe) $(test_tree_o) \
	-L$(lib) -ltree -lallocator_std -lm -ltree_iterator
	rm $(test_tree_o)
>>>>>>> tree
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
	-L$(lib) -lcontainer -ltree -lallocator_std -lm
	rm $(test_o)
####################################

####################################
# test_list
test_list_c = $(CURRENT_DIR)/test/test_list.c
test_list_o = $(CURRENT_DIR)/test/test_list.o
test_list_exe = $(CURRENT_DIR)/test/test_list

test_list_obj: $(test_list_c)
	gcc -Wall -$(CXX_FLAG) \
	-I $(include) \
	-I $(include)/list \
	-c $(test_list_c) \
	-o $(test_list_o)

test_list_exe: test_list_obj
	gcc -Wall -$(CXX_FLAG) \
	-o $(test_list_exe) $(test_list_o) \
	-L$(lib) -llist -lallocator_std
	rm $(test_list_o)
####################################

####################################
# test_hash_table
test_hash_table_c = $(CURRENT_DIR)/test/test_hash_table.c
test_hash_table_o = $(CURRENT_DIR)/test/test_hash_table.o
test_hash_table_exe = $(CURRENT_DIR)/test/test_hash_table

test_hash_table_obj: $(test_hash_table_c)
	gcc -Wall -$(CXX_FLAG) \
	-I $(include) \
	-I $(include)/hash_table \
	-I $(include)/array \
	-c $(test_hash_table_c) \
	-o $(test_hash_table_o)

test_hash_table_exe: test_hash_table_obj
	gcc -Wall -$(CXX_FLAG) \
	-o $(test_hash_table_exe) $(test_hash_table_o) \
	-L$(lib) -lhash_table -lallocator_std -larray -llist
	rm $(test_hash_table_o)
####################################

####################################
# test_array
test_array_c = $(CURRENT_DIR)/test/test_array.c
test_array_o = $(CURRENT_DIR)/test/test_array.o
test_array_exe = $(CURRENT_DIR)/test/test_array

test_array_obj: $(test_array_c)
	gcc -Wall -$(CXX_FLAG) \
	-I $(include) \
	-I $(include)/array \
	-c $(test_array_c) \
	-o $(test_array_o)

test_array_exe: test_array_obj
	gcc -Wall -$(CXX_FLAG) \
	-o $(test_array_exe) $(test_array_o) \
	-L$(lib) -larray -lallocator_std
	rm $(test_array_o)
# ========================
####################################
