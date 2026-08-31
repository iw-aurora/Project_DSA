# Makefile for Student Database Project

CXX = g++
CXXFLAGS = -std=c++11 -Wall
TARGET = main.exe
SRCS = main.cpp src/StudentDatabase.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

run: $(TARGET)
	.\$(TARGET)

clean:
	if exist $(TARGET) del /f $(TARGET)
