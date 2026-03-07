#include "pch.h"
#include "Unit.h"

Unit::Unit()
	:_hp(0), _id(0),_ownerId(0)
{
}

Unit::Unit(Vector2 pos, uint16 unitId, uint16 playerId)
	:_pos(pos), _id(), _ownerId()
{
	_hp = _maxHp;
}
