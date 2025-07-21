
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = LoadBalancer

SOURCES = main.cpp LoadBalancer.cpp WebServer.cpp Request.cpp

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET).exe $(SOURCES)

clean:
	del /Q *.exe 2>nul || echo "No files to clean"

run: $(TARGET)
	./$(TARGET).exe

.PHONY: clean run
