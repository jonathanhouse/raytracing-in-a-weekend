CXX := g++ 
CXXFLAGS := -std=c++11

SRCS := $(wildcard *.cpp)

MAIN_SRCS := $(SRCS)

TARGETS := $(patsubst %.cpp, %, $(MAIN_SRCS))

all: $(TARGETS)

	@echo $(TARGETS)

%: %.cpp 
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f $(TARGETS)


