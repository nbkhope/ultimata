Ultimata:	Creature.o Cursor.o GameMap.o Graphics.o Main.o HelperFunctions.o Input.o System.o Tile.o Tileset.o Timer.o
	g++ -o build/Ultimata build/*.o -lSDL2 -lSDL2_image

Creature.o: Creature.cpp Creature.h
	g++ -c Creature.cpp -o build/Creature.o

Cursor.o: Cursor.cpp Cursor.h
	g++ -c Cursor.cpp -o build/Cursor.o

GameMap.o: GameMap.cpp GameMap.h
	g++ -c GameMap.cpp -o build/GameMap.o

Graphics.o: Graphics.cpp Graphics.h
	g++ -c Graphics.cpp -o build/Graphics.o

HelperFunctions.o: HelperFunctions.cpp HelperFunctions.h
	g++ -c HelperFunctions.cpp -o build/HelperFunctions.o

Input.o: Input.cpp Input.h
	g++ -c Input.cpp -o build/Input.o

Main.o: Main.cpp Main.h
	g++ -c Main.cpp -o build/Main.o

System.o: System.cpp System.h
	g++ -c System.cpp -o build/System.o

Tile.o: Tile.cpp Tile.h
	g++ -c Tile.cpp -o build/Tile.o

Tileset.o: Tileset.cpp Tileset.h
	g++ -c Tileset.cpp -o build/Tileset.o

Timer.o: Timer.cpp Timer.h
	g++ -c Timer.cpp -o build/Timer.o

clean:
	$(RM) Ultimata *.o
