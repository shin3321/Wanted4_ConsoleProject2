#include "Unit.h"

Unit::Unit()
	: _id(-1), _ownerId(-1)
{
}

Unit::Unit(Vector2 pos, uint16_t ownerId)
	:super("U", pos, Color::Green)
{
	_pos = pos;
	_ownerId = ownerId;
}

Unit::Unit(Vector2 pos, Color color)
	:super("U", pos, Color::Red)
{
	_pos = pos;
}

Unit::~Unit()
{
}

void Unit::Select()
{
	color = Color::Purple;
	_isSelected = true;
}

void Unit::Unselect()
{
	color = Color::Green;
	_isSelected = false;
}
