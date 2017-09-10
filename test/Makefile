#BOOST_INC = -I/usr/include
#BOOST_LIB = -L/usr/lib/x86_64-linux-gnu

INCLUDES = -I../include $(BOOST_INC)
CXXFLAGS = -std=c++14 -g -O2 -Wall -Wextra -Wpedantic -Werror $(INCLUDES)
LDFLAGS = $(BOOST_LIB) -lboost_unit_test_framework
TESTFLAGS = --report_level=short

%.o: %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

%: %.o
	$(CXX) -o $@ $< $(LDFLAGS)

all: test_indexed_heap test_union_find

test_indexed_heap: test_indexed_heap.o
test_indexed_heap.o: ../include/indexed_heap.hpp

test_union_find: test_union_find.o
test_union_find.o: ../include/union_find.hpp

test: all
	./test_indexed_heap $(TESTFLAGS)
	./test_union_find $(TESTFLAGS)

clean:
	rm -f *.o test_indexed_heap test_union_find
