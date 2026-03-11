#pragma once
#include "01.Game/Actor/Unit.h"

class Castle: public Unit
{
public:
	Castle(Vector2 pos, uint16 ownerId);
	
	void attacked(uint16 attack);

};

