#pragma once
#include "Actor\Actor.h"
#include "Utils/Timer.h"
using namespace Wanted;

enum class NpcType { None, Small, Large };

class NPC : public Actor
{
	RTTI_DECLARATIONS(NPC, Actor)

public:
	struct StunFrame {
		const char* frame;
		float playTime;
		Color color;

		StunFrame(const char* f, float t, Color c)
			: frame(f), playTime(t), color(c) {
		}
	};


	NPC();
	NPC(int id, const Vector2 position);
	NPC(int id, const Vector2 position, NpcType type);

	virtual void Tick(float deltaTime) override;

	void SetNewPosition(Vector2 pos);

	// 스턴 관련 함수
	void PlayStunAnim();      // 스턴 시작
	void StopStunAnim();      // 스턴 종료 (원상복구)

private:
	// 스턴 애니메이션 로직 
	void UpdateStunAnim(float deltaTime);


private:
	int _id;
	Vector2 _pos;
	NpcType _type;

	// --- 스턴 애니메이션 관련 변수 ---
	bool _isStunned = false;        // 현재 스턴 상태인가?
	Timer _stunTimer;               // 타이머
	int _stunIndex = 0;             // 현재 프레임 인덱스

	// 모든 NPC가 공유하는 데이터이므로 static const로 선언
	static const StunFrame _stunSequence[];
	static const int _stunSequenceCount;
};

