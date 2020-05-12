CXX := g++
CFLAGS := -std=c++17 -pthread -W{error,all}
INCLUDE := -I ./

CLIENT := client.cpp
SOURCE := $(filter-out $(CLIENT), $(wildcard *.cpp))
OBJ := $(addsuffix .o, $(basename $(notdir $(SOURCE))))
SERVER := $(filter-out server_main, $(basename $(OBJ)))

all: $(SERVER)
	$(CXX) -o client $(CLIENT) $(CFLAGS) $(INCLUDE)

$(SERVER): $(OBJ)
	$(CXX) -o $@ server_main.o $(addsuffix .o, $@) $(CFLAGS)

$(OBJ): clean
	$(CXX) -c $(addsuffix .cpp, $(basename $@)) -o $@ $(CFLAGS) $(INCLUDE)

.PHONY: clean
clean:
	rm -f *.o
	rm -f client
	rm -f $(OBJ)
	rm -f $(SERVER)
