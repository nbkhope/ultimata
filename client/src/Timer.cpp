#include "Timer.h"

Timer::Timer()
{
	on = false;
	paused = false;
	startTicks = pausedTicks = 0;
}

bool Timer::isOn() const
{
	return on;
}

bool Timer::isPaused() const
{
	return paused;
}

int Timer::getTicks() const
{
	if (on)
	{
		if (!paused)
			return SDL_GetTicks() - startTicks;
		else
			return pausedTicks - startTicks;
			//return pausedTicks;
	}
	else // issue: if return 0, getTicks() will give zero after stopping timer
		 // that is, we won't know much time elapsed;
		 // fix: pause, get what you need, then stop timer.
		return 0;
}


void Timer::start()
{
	if(!on)
	{
		on = true;
		//paused = false;
		
		// SDL_GetTicks():
		// Returns an unsigned 32-bit value representing the number of milliseconds since the SDL library initialized.
		startTicks = SDL_GetTicks();
	}
	else if (paused)
	{
		paused = false;
		startTicks = SDL_GetTicks() - (pausedTicks - startTicks);
		pausedTicks = 0;
	}
#ifdef __DEBUG_MODE__
	cout << "Timer started @ " << SDL_GetTicks() << endl;
#endif
}

void Timer::pause()
{
	if(on && !paused)
	{
		paused = true;
		pausedTicks = SDL_GetTicks();
	}
#ifdef __DEBUG_MODE__
	cout << "Timer paused @ " << SDL_GetTicks() << endl;
	cout << "(Time elapsed: " << getTicks() / 1000. << " s)" << endl;
#endif
}

/*
void Timer::unpause()
{

}
*/

void Timer::stop()
{
	if (on)
	{
#ifdef __DEBUG_MODE__
		cout << "Timer stopped @ " << SDL_GetTicks() << endl;
		cout << "(Time elapsed: " << getTicks() / 1000. << " s)" << endl;
#endif
		on = false;
	
		if (paused)
			paused = false;
	}

}

void Timer::reset()
{
	//on = true;
	//if (!paused)
	//	paused = false;
/*	if (!paused)
		startTicks = SDL_GetTicks();
	else
		startTicks = 0;
	pausedTicks = 0;*/
#ifdef __DEBUG_MODE__
	cout << "Timer reset @ " << SDL_GetTicks() << endl;
	cout << "(Time elapsed: " << getTicks() / 1000. << " s)" << endl;
#endif
	pausedTicks = startTicks = SDL_GetTicks();
}

