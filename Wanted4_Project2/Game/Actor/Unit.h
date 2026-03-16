#pragma once
#include "Actor\Actor.h"
#include "Utils/Timer.h"
#include "Utils/AStar/Astar.h"

#include <iostream>
#include <vector>
#include <queue>

using namespace Wanted;

class Unit : public Actor
{
	RTTI_DECLARATIONS(Unit, Actor)

public:
	struct DamageFrame {
		const char* frame;
		float playTime;
		Color color;

		DamageFrame(const char* f, float t, Color c)
			: frame(f), playTime(t), color(c) {
		}
	};

	Unit();
	Unit(Vector2 pos, uint16_t unitId ,uint16_t ownerId);
	Unit(Vector2 pos, uint16_t unitId, Color color);
	~Unit();

	virtual void Tick(float deltaTime);

	void MoveUnit(std::vector<Vector2> path);
	void MoveUnit(Vector2 movePos);

	void MoveAStar(Vector2 goalPos);
	void move();
	void showMove();

	void Select();
	void Unselect();

	//애니메이션 관련 함수
	void PlayDamageAnim();
	void UpdateDamageAnim(float deltaTime);
	void StopDamageAnim();

	uint16_t _id;
	uint16_t _ownerId = -1;

	//위치
	Vector2 _pos;
	Color _color;

	//// 체력 (HP바 표시용)
	//int _hp;
	//int _maxHp;

	// 선택 여부 (드래그 선택)
	bool _isSelected = false;

	Timer _timer;
	float _moveTimer = 0.05f;
	bool _isMoving = true;

	//AStar 관련
	AStar _pathFinder;
	
	//std::queue<Vector2> _path = {};
	std::vector<Vector2> _path = {};
	int _pathIndex = 0;

	// --- 데미지 애니메이션 관련 변수 ---
	bool _isDamaged = false;        // 현재 데미지를 받은 상태인가?
	Timer _damagedTimer;               // 타이머
	int _damagedIndex = 0;             // 현재 프레임 인덱스


	// 모든 NPC가 공유하는 데이터이므로 static const로 선언
	static const DamageFrame _DamagedSequence[];
	static const int _DamagedSequenceCount;
};

