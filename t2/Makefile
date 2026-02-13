CXX := g++
CXXFLAGS := -std=c++17 -O2
LIBS := -lGL -lGLU -lglut

TARGET := trabalhocg
SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:.cpp=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(TARGET) $(OBJS)
