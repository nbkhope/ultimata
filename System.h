//	Copyright (C) 2010-2011, 2015  nbkhope
//
//	This program is free software; you can redistribute it and/or
//	modify it under the terms of the GNU General Public License
//	as published by the Free Software Foundation; either version 2
//	of the License, or (at your option) any later version.
//
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with this program; if not, write to the Free Software
//	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

/** @file
 */

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <iostream>

/*#ifdef __APPLE__
	#include "TargetConditionals.h"
	#if TARGET_OS_MAC
		#include "SDL2/SDL.h"
	#endif*/
#ifdef __OSX__
	#include "SDL2/SDL.h"
#elif __linux
	// Linux
#elif _WIN32
	// WIndows
#endif

#include "Constants.h"
#include "Graphics.h"

using namespace std;

/**
 * Responsible for initializing the SDL API
 * and setting up the game
 */
class System
{
public:
	/**
	 * Constructor and destructor
	 */
	System();
	~System();
	
	/**
	 * Initializes SDL and its subsystems
	 */
	int init(Graphics* graphics);

private:
	
};

#endif
