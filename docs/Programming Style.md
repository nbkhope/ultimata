# Programming Style
## Comments
* Use doxygen style comments
* if, elseif, for, while, etc must have a space before the opening parenthesis
```
	if (myInteger == 2) ...
```
* function calls do not follow the previous rule
```
	int getHealth()
	{ 
		... 
	}
	...
	... getHealth();

	void setHealth(int newHealth)
	{
		...
	}
```
* always place opening braces in a new line, as shown above in the setHealth() function

## File placement
* All class declarations should go into a **header file** named after the class
  e.g. **Creature.h** for the __Creature__ class declaration
* All class definitions should go into a **.cpp implementation file**
  e.g. Definitions for Creature class functions should go into **Creature.cpp**

### Other placements
* documentation: docs/
* development tools: tools/
* images: data/images/
* game maps: data/maps/

