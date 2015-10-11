# Documentation
## How to Compile
### Using MAC OS X and Xcode:
You will need:

* [SDL 2.0](https://www.libsdl.org/download-2.0.php "SDL 2.0")
(SDL2-2.0.3.dmg)
* [SDL image 2.0](https://www.libsdl.org/projects/SDL_image/ "SDL_image")
(SDL2_image-2.0.0.dmg)
* [SDL ttf 2.0](https://www.libsdl.org/projects/SDL_ttf/ "SDL_ttf")
(SDL2_ttf-2.0.12.dmg)
* [SDL mixer 2.0](https://www.libsdl.org/projects/SDL_mixer/ "SDL_mixer")
(SDL2_mixer-2.0.0.dmg)

After downloading all the files, place the framework files in
/Library/Frameworks

In Xcode, find the tab **Build Phases** (under the **Targets** 2 - Ultimata 2 project name), locate the "Link Binary With Libraries" section and click + to add the frameworks you downloaded above.

Under the Project settings, in the **Build Settings** tab, you might need to set "Symbols Hidden by Default" to "No", under the "Apple LLVM 7.0 - Code Generation" section.

Edit your project **Scheme** and mark "Use custom working directory" under the Run section, Options tab. Point to the directory with the source files.