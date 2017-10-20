#BOOST_INC = -I/usr/include
#BOOST_LIB = -L/usr/lib/x86_64-linux-gnu
GOOGLE_BENCHMARK_DIR = $(HOME)/benchmark

INCLUDES = -I../include $(BOOST_INC)
CXXFLAGS = -std=c++14 -g -O2 -Wall -Wextra -Wpedantic -Werror $(INCLUDES)
LDFLAGS = $(BOOST_LIB) -lboost_unit_test_framework
TESTFLAGS = --catch_system_error=yes --report_level=short

all: test_indexed_heap test_union_find bm_indexed_heap bm_union_find

%.o: %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

%: %.o
	$(CXX) -o $@ $< $(LDFLAGS)

bm_%: bm_%.cpp
	$(CXX) -o $@ $< $(CXXFLAGS) -pthread -I$(GOOGLE_BENCHMARK_DIR)/include $(GOOGLE_BENCHMARK_DIR)/build/src/libbenchmark.a

test_indexed_heap: test_indexed_heap.o

test_indexed_heap.o: ../include/indexed_heap.hpp

bm_indexed_heap: ../include/indexed_heap.hpp

test_union_find: test_union_find.o

test_union_find.o: ../include/union_find.hpp

bm_union_find: ../include/union_find.hpp

test: test_indexed_heap test_union_find
	./test_indexed_heap $(TESTFLAGS)
	./test_union_find $(TESTFLAGS)

memcheck: test_indexed_heap test_union_find
	valgrind --leak-check=full ./test_indexed_heap $(TESTFLAGS)
	valgrind --leak-check=full ./test_union_find $(TESTFLAGS)

bm: bm_indexed_heap bm_union_find
	./bm_indexed_heap
	./bm_union_find

clean:
	rm -f *.o test_indexed_heap test_union_find bm_indexed_heap bm_union_find
