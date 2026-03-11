#include "pch.h"
#include "Castle.h"
#include "01.Game/Game.h"

Castle::Castle(Vector2 pos, uint16 ownerId)
{
    _maxHp = 100;
    _hp = _maxHp;
	_pos = pos;
	_ownerId = ownerId;
    _id = ownerId;
}

void Castle::attacked(uint16 attack)
{
	_hp -= attack;
    if (_hp == 0)
    {
        _isAlive = false;
        Game::get().removeCastle(_id, _ownerId);
    }
}
