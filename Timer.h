//	Copyright (C) 2010-2011, 2015  Renan Martins
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

#ifndef __TIMER_H__
#define __TIMER_H__

#include <iostream>

#include <SDL2/SDL.h>

#include "Constants.h"

using namespace std;

class Timer
{
public:
	/**
	 * Constructor
	 */
	Timer();

	/**
	 * Determines whether the timer is activated.
	 */
	bool isOn() const;

	/**
	 * Determines whether the timer is paused.
	 */
	bool isPaused() const;

	/**
	 * @return the number of milliseconds elapsed since the timer has started.
	 */
	int getTicks() const;

	/**
	 * Starts the timer or unpauses it if it is paused.
	 */
	void start();

	/**
	 * Pauses the timer.
	 */
	void pause();

	//void unpause();
	
	/**
	 * Stops the timer.
	 */
	void stop();

	/**
	 * Resets the timer but does not stop or unpause it.
	 */
	void reset();
	
private:
	/**
	 * Variables to hold the state of the timer
	 */
	bool on, paused;

	/**
	 * The number of ticks in milliseconds when the timer was started
	 * and when it was paused.
	 */ 
	int startTicks, pausedTicks;
};

#endif
