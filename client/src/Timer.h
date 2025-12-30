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
