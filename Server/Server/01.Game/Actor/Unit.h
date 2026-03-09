#pragma once
#include "01.Game/AStar/Astar.h"

class Unit
{
public:
	Unit();
	Unit(Vector2 pos, uint16 unitId, uint16 playerId);

	//astar 맵 전달
	void setMap(std::vector<uint8>& mapData) {
		_aStar.setMapData(mapData);
	}

	//유닛 움직이기
	std::vector<Vector2> moveUnit(Vector2 goalPos);

	void takeDamage(uint16 attack);
	void despawnUnit();

	uint16 getId() { return _id; }

public:
	uint16 _id;
	uint16 _ownerId;        // 어느 플레이어 소유

	// 위치
	Vector2 _pos;
	Vector2 _targetPos; // 목적지

	// 스탯
	int _hp;
	int _maxHp = 100;
	int _attack = 5;
	int _range = 3;        // 공격 사거리

	// 상태
	bool _isMoving = false;
	bool _isAttacking = false;
	bool _isAlive;
	uint16 _attackTargetId = 0;  // 공격 중인 대상

	AStar _aStar;
	// A* 경로
	std::vector<Vector2> _path;

};