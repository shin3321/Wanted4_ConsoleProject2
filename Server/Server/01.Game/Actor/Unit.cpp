#include "pch.h"
#include "Unit.h"
#include "01.Game/Game.h"
Unit::Unit()
    :_hp(0), _id(0), _ownerId(0), _isAlive(true)
{
}

Unit::Unit(Vector2 pos, uint16 unitId, uint16 playerId)
	:_pos(pos), _id(unitId), _ownerId(playerId)
{
	_hp = _maxHp;
}

std::vector<Vector2> Unit::moveUnit(Vector2 goalPos)
{
    _path = _aStar.FindPath(_pos, goalPos, _id);

    if (_path.empty())
    {
        return{};
    } 
    return _path;
}

void Unit::takeDamage(uint16 attack)
{
    _hp -= attack;
    if (_hp == 0)
    {
        _isAlive = false;    
        Game::get().removeUnit(_id, _ownerId);
    }
}