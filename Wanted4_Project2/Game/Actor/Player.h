#pragma once

#include "Actor/Actor.h"
#include "Utils/Timer.h"

#include <functional>
#include <unordered_set>

using namespace Wanted;

class Unit;
class INetwork;
class Player : public Actor
{
	// 발사 모드.
	enum class FireMode
	{
		None = -1,
		OneShot,
		Repeat
	};

	RTTI_DECLARATIONS(Player, Actor)

public:
	Player();
	Player(uint16_t id);
	~Player();

public:
	virtual void Tick(float deltaTime) override;

	void UpdateDragSelection(Vector2 mousePos);
	void FinishDragSelection(Vector2 mousePos);
	void RenderDragBox();

	void SpawnUnit(Vector2 mousePos);
	void MoveUnit(Vector2 mousePos);

	void AddUnit(uint16_t unitId);

public:
	//아이디 지정 함수
	void SetId(uint16_t id) { _id = id; }
	uint16_t GetId() { return _id; }

public:
	void SetSender(INetwork* networker) { _networker = networker; }

private:
	//네트워크 통신을 위한 클래스
	INetwork* _networker = nullptr;

	// 타이머 변수.
	Timer timer;

	std::unordered_set<uint16_t> _myUnitIds;      // 내 유닛 ID 목록
	std::unordered_map<uint16_t, std::shared_ptr<Unit>> _selectedUnitIds; // 선택된 유닛 ID 목록

	// 드래
	Vector2 _dragStart;
	Vector2 _dragEnd;
	bool    _isDragging = false;

	int minX = 0;
	int minY = 0;
	int maxX = 0;
	int maxY = 0;
	uint16_t _id;
};