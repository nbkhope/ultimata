# NOTE: This Makefile is kept for backward compatibility
# For modern development, use vcpkg and CMake:
#   1. Run install.bat and build.bat to install dependencies and build
#   2. Run startit.bat to start the server
#
FLAGS=-IC:\Users\nkans\include -LC:\Users\nkans\lib

OBJS=build/Main.o
DEPS=$(OBJS:.o=.d)

Server: $(OBJS)
	g++ $(FLAGS) -o build/server.exe $(OBJS)

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
