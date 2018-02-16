LDFLAGS = -pthread -rdynamic
CXXFLAGS = \
	-g -D_GNU_SOURCE \
	-I.

CXXFLAGS += -Wall --std=c++11
#CXXFLAGS += -O3

EXAMPLE = \
	example.o \

PROGRAMS = \
	example \

all: $(PROGRAMS)

example: $(EXAMPLE)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -rf $(PROGRAMS) ./*.o ./*.so ./*/*.o ./*/*.so
