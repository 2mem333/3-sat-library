CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2 -static  

TARGET = sat3_solver.exe

SRCS = Source.cpp
OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(CXXFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	del /F /Q $(OBJS) $(TARGET) 2> nul || rm -f $(OBJS) $(TARGET)
