raytracer: main.cpp vector.o camera.o
	g++ vector.o -Wall -Wextra -Wpedantic -std=c++17 -O2 main.cpp -o raytracer
camera.o: camera.cpp vector.o
	g++ vector.o -c -Wall -Wextra -Wpedantic -std=c++17 -O2 camera.cpp -o camera.o
vector.o: vector.cpp vector.h
	g++ -c -Wall -Wextra -Wpedantic -std=c++17 -O2 vector.cpp -o vector.o
