#include "Castle.h"
#include "Render/Renderer.h"

const Castle::DamageFrame Castle::_DamagedSequence[] =
{
	Castle::DamageFrame("C", 0.1f, Color::BackgroundRed),
	Castle::DamageFrame("C", 0.1f, Color::LightBlue),
	Castle::DamageFrame("C", 0.1f, Color::Blue),
	Castle::DamageFrame("C", 0.1f, Color::BackgroundRed),
	Castle::DamageFrame("C", 0.1f, Color::Purple),
	Castle::DamageFrame("C", 0.1f, Color::Green)
};
const int Castle::_DamagedSequenceCount = sizeof(Castle::_DamagedSequence) / sizeof(Castle::_DamagedSequence[0]);

Castle::Castle()
{

}

Castle::Castle(Vector2 castlePos, uint16_t castleId, uint16_t ownerId)
	:super("C", castlePos, Color::Blue)
{
	_ownerId = ownerId;
	_castleId = castleId;
	_pos = castlePos;
}

Castle::~Castle()
{
}

void Castle::Tick(float deltaTime)
{
	super::Tick(deltaTime);
	_timer.Tick(deltaTime);

	if (_isDamaged)
	{
		UpdateDamageAnim(deltaTime);
	}
}

void Castle::Draw()
{
	super::Draw();
}

void Castle::PlayDamageAnim()
{
	if (_isDamaged) return;

	_isDamaged = true;
	_damagedIndex = 0;

	// 첫 번째 프레임 적용
	_damagedTimer.SetTargetTime(_DamagedSequence[0].playTime);
	_damagedTimer.Reset();

	ChangeImage(_DamagedSequence[0].frame); // Actor의 이미지 변경
	color = _DamagedSequence[0].color;      // Actor의 색상 변경
}

void Castle::UpdateDamageAnim(float deltaTime)
{
	_damagedTimer.Tick(deltaTime);

	if (!_damagedTimer.IsTimeOut())
	{
		return;
	}

	//다음 프레임으로 넘기기 전, 마지막인지 체크
	if (_damagedIndex >= _DamagedSequenceCount - 1)
	{
		_damagedIndex = 0;
		StopDamageAnim();
		return;
	}
	else
	{
		_damagedIndex++;
	}

	_damagedTimer.Reset();
	_damagedTimer.SetTargetTime(_DamagedSequence[_damagedIndex].playTime);

	ChangeImage(_DamagedSequence[_damagedIndex].frame);
	color = _DamagedSequence[_damagedIndex].color;
}

void Castle::StopDamageAnim()
{
	_isDamaged = false;

	ChangeImage("U");
	color = Color::BackgroundBlue;
}
