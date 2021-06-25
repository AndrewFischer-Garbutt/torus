CC=g++
LIBS := -L. -lglfw3 -lGL -lglut -lGLEW -lX11 -lpthread -lXrandr -lXi -ldl

main: shaders.o main.o torus.o
	g++  obj/torus.o obj/shaders.o obj/main.o -o $@ $(LIBS)

shaders.o: shaders.h shaders.cpp
	g++ -c shaders.cpp -o ./obj/$@

main.o: main.cpp
	g++ -c main.cpp -o ./obj/$@

torus.o: torus.h torus.cpp
	g++ -c torus.cpp -o ./obj/$@

run: main
	./main

clean:
	rm obj/* main
