#pragma once
#include "Actor\Actor.h"

using namespace Wanted;
class Castle : public Actor
{
	RTTI_DECLARATIONS(Castle, Actor)
public:
	Castle();
	Castle(Vector2 castlePos);
	~Castle();

private:
	Vector2 _pos;

};

