all:snakegame

snakegame: main.o game.o snake.o
	g++ -o snakegame main.o game.o snake.o -lcurses
main.o: main.cpp game.cpp
	g++ -c main.cpp
game.o: game.cpp snake.cpp
	g++ -c game.cpp
snake.o: snake.cpp
	g++ -c snake.cpp
clean:
	rm *.o 
	rm snakegame
	rm record.dat
