CXX := g++
CFLAGS := -std=c++17

SOURCE := $(wildcard *.cpp)
OBJ := $(basename $(notdir $(SOURCE)))

all: $(OBJ)

$(OBJ): clean
	$(CXX) -o $@ $(addsuffix .cpp, $@) $(CFLAGS)

.PHONY: clean
clean:
	rm -f *.o
	rm -f $(OBJ)
