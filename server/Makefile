# SDL2_net is required for this server to compile
# If you get "SDL2/SDL_net.h: No such file or directory", you need to install SDL2_net development headers
# Download SDL2_net from: https://www.libsdl.org/projects/SDL_net/
FLAGS=-IC:\Users\nkans\include -LC:\Users\nkans\lib

OBJS=build/Main.o
DEPS=$(OBJS:.o=.d)

Server: $(OBJS)
	g++ $(FLAGS) -o build/server.exe $(OBJS) -lSDL2main -lSDL2 -lSDL2_net

run: Server
	@echo Starting server...
	build/server.exe

build:
	@if not exist build mkdir build

build/%.o: src/%.cpp | build
	g++ $(FLAGS) -MMD -MP -c $< -o $@

clean:
	@if exist build\server.exe del /Q build\server.exe
	@if exist build\*.o del /Q build\*.o
	@if exist build\*.d del /Q build\*.d
	@if exist build rmdir /S /Q build

.PHONY: clean run

-include $(DEPS)
