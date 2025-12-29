#include "Monster.h"

#include "constants.h"

Monster::Monster() : Creature()
{
	// Use public setters instead of accessing private members
	setPos(64, 64);
}

Monster::~Monster()
{

}
