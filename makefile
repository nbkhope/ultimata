
FLAGS=-IC:\Users\nkans\include -LC:\Users\nkans\lib

OBJS= \
	build/Creature.o \
	build/Cursor.o \
	build/GameMap.o \
	build/Graphics.o \
	build/Main.o \
	build/HelperFunctions.o \
	build/Input.o \
	build/System.o \
	build/Tile.o \
	build/Tileset.o \
	build/Timer.o \
	build/Widget.o \
	build/Sound.o \
	build/Monster.o \
	build/Camera.o

DEPS=$(OBJS:.o=.d)

Ultimata: $(OBJS)
	g++ $(FLAGS) -o build/Ultimata $(OBJS) -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2_net
	copy dll\*.dll build

build:
	@if not exist build mkdir build

build/%.o: src/%.cpp | build
	g++ $(FLAGS) -MMD -MP -c $< -o $@

clean:
	@if exist build\Ultimata del /Q build\Ultimata
	@if exist build\*.o del /Q build\*.o
	@if exist build\*.d del /Q build\*.d
	@if exist build rmdir /S /Q build

.PHONY: clean

-include $(DEPS)
