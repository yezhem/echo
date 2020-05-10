CXX := g++
CFLAGS := -std=c++17 -pthread -W{error,all}

SOURCE := $(wildcard *.cpp)
OBJ := $(basename $(notdir $(SOURCE)))

all: $(OBJ)

$(OBJ): clean
	$(CXX) -o $@ $(addsuffix .cpp, $@) $(CFLAGS)

.PHONY: clean
clean:
	rm -f *.o
	rm -f $(OBJ)
