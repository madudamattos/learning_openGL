CXX := g++
CXXFLAGS := -std=c++17 -O2
INCLUDES := -I. -Icomponents -Imanagers -Iutils
LIBS := -lGL -lGLU -lglut

TARGET := trabalhocg3D
SRCS := $(wildcard *.cpp components/*.cpp managers/*.cpp utils/*.cpp)
OBJS := $(SRCS:.cpp=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
