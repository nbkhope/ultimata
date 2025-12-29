# SDL2_net is required for this server to compile
# If you get "SDL2/SDL_net.h: No such file or directory", you need to install SDL2_net development headers
# Download SDL2_net from: https://www.libsdl.org/projects/SDL_net/
FLAGS=-IC:\Users\nkans\include -LC:\Users\nkans\lib

Server: Main.o
	g++ $(FLAGS) -o server.exe Main.o -lSDL2main -lSDL2 -lSDL2_net

Main.o: Main.cpp
	g++ $(FLAGS) -c Main.cpp
