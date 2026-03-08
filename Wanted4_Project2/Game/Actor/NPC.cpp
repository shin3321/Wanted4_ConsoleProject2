#include "NPC.h"

const NPC::StunFrame NPC::_stunSequence[] =
{
	NPC::StunFrame("t", 0.1f, Color::Red),
	NPC::StunFrame("T", 0.1f, Color::Blue),
	NPC::StunFrame("T", 0.1f, Color::Green),
	NPC::StunFrame("t", 0.1f, Color::Red),
	NPC::StunFrame("t", 0.1f, Color::Purple),
};

const int NPC::_stunSequenceCount = sizeof(NPC::_stunSequence) / sizeof(NPC::_stunSequence[0]);

NPC::NPC()
	: _id(0), _type(NpcType::None), _stunTimer(0.0f)
{
}

NPC::NPC(int id, const Vector2 position)
	: super("t", position, Color::Red), _id(id), _pos(position), _type(NpcType::Small), _stunTimer(0.0f)
{
	// 그리기 우선 순위 지정 
	sortingOrder = 4;
	_pos = position;
}

NPC::NPC(int id, const Vector2 position, NpcType type)
	: super("T", position, Color::Red), _id(id), _pos(position), _type(type), _stunTimer(0.0f)
{
	sortingOrder = 5;
}

void NPC::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	if (_isStunned)
	{
		UpdateStunAnim(deltaTime);
	}
}

void NPC::SetNewPosition(Vector2 pos)
{
	SetPosition(pos);
}

void NPC::PlayStunAnim()
{
	if(_isStunned) return; // 이미 스턴이면 무시

	_isStunned = true;
	_stunIndex = 0;

	// 첫 번째 프레임 적용
	_stunTimer.SetTargetTime(_stunSequence[0].playTime);
	_stunTimer.Reset();

	ChangeImage(_stunSequence[0].frame); // Actor의 이미지 변경
	color = _stunSequence[0].color;      // Actor의 색상 변경
}

void NPC::StopStunAnim()
{
	_isStunned = false;

	ChangeImage("T");
	color = Color::Red;
}

void NPC::UpdateStunAnim(float deltaTime)
{
	_stunTimer.Tick(deltaTime);

	if (!_stunTimer.IsTimeOut())
	{
		return;
	}

	// 다음 프레임으로 넘기기 전, 마지막인지 체크
	if (_stunIndex >= _stunSequenceCount - 1)
	{
		// 반복 재생할 거면 0으로, 아니면 StopStunAnim() 호출
		_stunIndex = 0;
	}
	else
	{
		_stunIndex++;
	}

	//타이머 리셋 및 다음 시간 설정
	_stunTimer.Reset();
	_stunTimer.SetTargetTime(_stunSequence[_stunIndex].playTime);

	//비주얼 적용 (이미지, 색상)
	ChangeImage(_stunSequence[_stunIndex].frame);
	color = _stunSequence[_stunIndex].color;
}
