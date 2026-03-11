#pragma once
#include "Actor\Actor.h"
#include "Utils/Timer.h"

#include <iostream>
using namespace Wanted;

class Castle : public Actor
{
	RTTI_DECLARATIONS(Castle, Actor)

public:
	struct DamageFrame {
		const char* frame;
		float playTime;
		Color color;

		DamageFrame(const char* f, float t, Color c)
			: frame(f), playTime(t), color(c) {
		}
	};

	Castle();
	Castle(Vector2 castlePos, uint16_t castleId, uint16_t ownerId);
	~Castle();

	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

	//애니메이션 관련 함수
	void PlayDamageAnim();
	void UpdateDamageAnim(float deltaTime);
	void StopDamageAnim();

private:
	// --- 데미지 애니메이션 관련 변수 ---
	bool _isDamaged = false;        // 현재 데미지를 받은 상태인가?
	Timer _damagedTimer;               // 타이머
	int _damagedIndex = 0;             // 현재 프레임 인덱스


	// 모든 NPC가 공유하는 데이터이므로 static const로 선언
	static const DamageFrame _DamagedSequence[];
	static const int _DamagedSequenceCount;

private:
	Timer _timer;
	Vector2 _pos;
	uint16_t _ownerId;
	uint16_t _castleId;
};

