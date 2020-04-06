
bomber: main.o MySDL.o Square.o Display.o Field.o Entity.o Bomber.o Bomb.o AI.o NetworkModule.o
	g++ -Wall -g -o bomber main.o Square.o Display.o MySDL.o Field.o Entity.o Bomber.o Bomb.o AI.o NetworkModule.o -lsfml-system -lsfml-network -lSDL_image -lSDL_gfx -lSDL_ttf -lSDL_mixer `sdl-config --cflags --libs` 

MySDL.o: MySDL.cpp
	g++ -Wall -g -o MySDL.o -c MySDL.cpp

main.o: main.cpp MySDL.h Field.h
	g++ -Wall -g -o main.o -c main.cpp

Field.o: Field.h
	g++ -Wall -g  -o Field.o -c Field.cpp

Square.o: Square.h
	g++ -Wall -g -o Square.o -c Square.cpp

Entity.o: Entity.h
	g++ -Wall -g -o Entity.o -c Entity.cpp

Bomber.o: Bomber.h Entity.o
	g++ -Wall -g -o Bomber.o -c Bomber.cpp

Bomb.o: Bomb.h Entity.o
	g++ -Wall -g -o Bomb.o -c Bomb.cpp
	
NetworkModule.o: NetworkModule.h
	g++ -Wall -g -o NetworkModule.o -c NetworkModule.cpp -lsfml-system -lsfml-network
	
AI.o: AI.h
	g++ -Wall -g -o AI.o -c AI.cpp


Display.o: Display.h
	g++ -Wall -g -o Display.o -c Display.cpp -lSDL_image -lSDL_gfx -lSDL_ttf -lSDL_mixer `sdl-config --cflags --libs` 
	

run: bomber
	./bomber

clear:
	rm -rf *.o
	
clean:
	rm -rf *.o

