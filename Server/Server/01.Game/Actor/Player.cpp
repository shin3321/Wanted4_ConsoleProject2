#include "pch.h"
#include "Player.h"

Player::Player(std::string nickname)
{
	_nickname = nickname;
}

void Player::addUnit(uint16 unitId, std::shared_ptr<Unit> unit)
{
	_units.try_emplace(unitId, unit);
}
