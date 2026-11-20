CXX=g++
CXXFLAGS=-std=c++17 -Wall -Wextra
TARGET=space_invaders
SRC=src/main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) -lncurses

clean:
	rm -f $(TARGET)
