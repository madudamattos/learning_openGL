CXX := g++
CXXFLAGS := -std=c++17 -O2
INCLUDES := -I. -Icomponents -Imanagers -Iutils
LIBS := -lGL -lGLU -lglut

TARGET := trabalhocg3D
SRCS := $(wildcard *.cpp components/*.cpp managers/*.cpp utils/*.cpp)

OBJDIR := objects
OBJS := $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCS))

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(TARGET)
	rm -rf $(OBJDIR)
