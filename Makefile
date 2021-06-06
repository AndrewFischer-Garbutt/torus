CXXFLAGS := -Wall -ggdb
LIBS := -lglfw3 -lGL -lglut -lGLEW -lX11 -lpthread -lXrandr -lXi -ldl
CXX := g++

main: *.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LIBS)
run: main
	./main
