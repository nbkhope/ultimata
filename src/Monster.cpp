#include "Monster.h"

#include "constants.h"

Monster::Monster() : Creature()
{
	name = "Monster";

	pos.x = 64;
	pos.y = 64;
	pos.w = 64;
	pos.h = 64;	
}

Monster::~Monster()
{

}

