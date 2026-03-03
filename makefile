CXX = g++

CXXFLAGS = -Wall -Wextra -std=c++11

TARGET = znack

SRCS = main.cpp ./utils/funcTerminal.cpp ./utils/funcUtilitarias.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)
%.o: %.cpp ./utils/funcTerminal.hpp ./utils/funcUtilitarias.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	clear
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
.PHONY: all run clean
