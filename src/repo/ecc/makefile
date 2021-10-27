CXX      = g++
CXX_FILE = $(wildcard *.cpp)
TARGET   = $(patsubst %.cpp,%,$(CXX_FILE))
CXXFLAGS = -g -std=c++11 -Wall -Werror -pedantic-errors -fmessage-length=0
INCLUDES = -I$(HOME)/libec/src/libec

all:
	$(CXX) $(CXXFLAGS) $(CXX_FILE) $(INCLUDES) -o $(TARGET)
clean:
	rm -f $(TARGET) $(TARGET).exe
