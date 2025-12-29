![Ultimata 2 Screenshot](docs/ultimata2.png "Ultimata 2")

# Ultimata 2

Game programming experiments using SDL 2.0.

## Goal

Ultimata is a project to build a simple two-dimensional role-playing game, drawing great influence from [Tibia](http://www.tibia.com "Tibia MMORPG") and the [Final Fantasy](https://en.wikipedia.org/wiki/Final_Fantasy "Final Fantasy series") series.

# Simple DirectMedia Layer

For more information about SDL, visit http://www.libsdl.org

## Git Diff with Old Mode 100755 / New Mode 100644

The following command was set on this repository to eliminate the files from git status:

```
git config core.filemode false
```

## MAC OS X Work Environment

When I first started writing Ultimata, I used Dev-Cpp on a Windows machine. To get the project to work with MAC OS X Yosemite, the following had to be done:
- Get the SDL frameworks to the directory /Library/Frameworks (this includes SDL_image, SDL_ttf, SDL_mixer)
- Change all the #include headers to point to
SDL/SDL.h
SDL_image/SDL_image.h
SDL_ttf/SDL_ttf.h
SDL_mixer/SDL_mixer.h
- Add the frameworks to the build options (Build Phases)
- Change Framework Search Paths to /Library/Frameworks
- Change "Objective-C Automatic Reference Counting" to No in Apple LLVM 6.1
- Use custom working directory (Product->Scheme->Edit Scheme, under Options) 
~/dev/Ultimata/Ultimata

## Setting up bzr

go to branch directory, then

```
bzr init 
bzr add *
bzr commit -m "Initial commit"
bzr push lp:~nbkhope/ultimata/2.0
```

```
git init
git add *
git commit -m "Initial commit"

git remote add github git@github.com:nbkhope/ultimata.git
git clone git@github.com:nbkhope/ultimata.git

git push ultimata master
```
