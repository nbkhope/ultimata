#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Creature.h"
#include "Map.h"
#include "Message.h"

class Player : public Creature
{
public:
	Player();
	~Player();

	void checkMovement(SDL_Event &event);
	//void move(Map &map); //, SDL_Rect &block); // I had this for a collision experiment
						  // maybe the map should move the player?

	bool getStats() const { return stats; }
	void setStats();
	void displayStats(Message* message, SDL_Surface* surface) const;

private:
	bool stats;

};

#endif
