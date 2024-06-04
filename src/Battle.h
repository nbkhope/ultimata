#ifndef _BATTLE_H_
#define _BATTLE_H_

class Battle
{
public:
	Battle();
	~Battle();

	bool combat(list<Character> &charList, list<Character> &monList);
private:
//	list<Character> charList;
//	list<Character> monList;
};

#endif
