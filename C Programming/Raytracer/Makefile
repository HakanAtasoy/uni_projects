CXX = g++
CXXFLAGS = -std=c++17 -O3

SOURCES = raytracer.cpp ppm.cpp parser.cpp tinyxml2.cpp
HEADERS = ppm.h parser.h tinyxml2.h raytracer.h
OBJECTS = $(SOURCES:.cpp=.o)

EXECUTABLE = raytracer

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ 

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
