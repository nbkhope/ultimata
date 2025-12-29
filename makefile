
FLAGS=-IC:\Users\nkans\include -LC:\Users\nkans\lib

Ultimata:	Creature.o Cursor.o GameMap.o Graphics.o Main.o HelperFunctions.o Input.o System.o Tile.o Tileset.o Timer.o Widget.o
	g++ $(FLAGS) -o build/Ultimata build/*.o -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_net
	copy dll\*.dll build

Creature.o: src/Creature.cpp src/Creature.h
	g++ $(FLAGS) -c src/Creature.cpp -o build/Creature.o

Cursor.o: src/Cursor.cpp src/Cursor.h
	g++ $(FLAGS) -c src/Cursor.cpp -o build/Cursor.o

GameMap.o: src/GameMap.cpp src/GameMap.h
	g++ $(FLAGS) -c src/GameMap.cpp -o build/GameMap.o

Graphics.o: src/Graphics.cpp src/Graphics.h
	g++ $(FLAGS) -c src/Graphics.cpp -o build/Graphics.o

HelperFunctions.o: src/HelperFunctions.cpp src/HelperFunctions.h
	g++ $(FLAGS) -c src/HelperFunctions.cpp -o build/HelperFunctions.o

Input.o: src/Input.cpp src/Input.h
	g++ $(FLAGS) -c src/Input.cpp -o build/Input.o

Main.o: src/Main.cpp src/Main.h
	g++ $(FLAGS) -c src/Main.cpp -o build/Main.o

System.o: src/System.cpp src/System.h
	g++ $(FLAGS) -c src/System.cpp -o build/System.o

Tile.o: src/Tile.cpp src/Tile.h
	g++ $(FLAGS) -c src/Tile.cpp -o build/Tile.o

Tileset.o: src/Tileset.cpp src/Tileset.h
	g++ $(FLAGS) -c src/Tileset.cpp -o build/Tileset.o

Timer.o: src/Timer.cpp src/Timer.h
	g++ $(FLAGS) -c src/Timer.cpp -o build/Timer.o

Widget.o: src/Widget.cpp src/Widget.h
	g++ $(FLAGS) -c src/Widget.cpp -o build/Widget.o

clean:
	$(RM) build/Ultimata build/*.o
