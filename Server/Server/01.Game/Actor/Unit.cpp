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

std::vector<Vector2> Unit::moveUnit(Vector2 goalPos)
{
    _path = _aStar.FindPath(_pos, goalPos);

    if (_path.empty())
    {
        // 경로 못 찾음 - 클라에 실패 응답
        return{};
    }
    _pos = goalPos;
    return _path;
}
