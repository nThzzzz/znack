CXX = g++

CXXFLAGS = -Wall -Wextra -std=c++11

TARGET = znack

SRCS = main.cpp funcTerminal.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)
%.o: %.cpp funcTerminal.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	clear
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
.PHONY: all run clean
