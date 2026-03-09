#include "Unit.h"

const Unit::DamageFrame Unit::_DamagedSequence[] =
{
	Unit::DamageFrame("U", 0.1f, Color::Green),
	Unit::DamageFrame("U", 0.1f, Color::LightBlue),
	Unit::DamageFrame("u", 0.1f, Color::Blue),
	Unit::DamageFrame("U", 0.1f, Color::LightBlue),
	Unit::DamageFrame("U", 0.1f, Color::Blue),
	Unit::DamageFrame("U", 0.1f, Color::Green)
};
const int Unit::_DamagedSequenceCount = sizeof(Unit::_DamagedSequence) / sizeof(Unit::_DamagedSequence[0]);


Unit::Unit()
	: _id(-1), _ownerId(-1)
{
}

Unit::Unit(Vector2 pos, uint16_t ownerId)
	:super("U", pos, Color::Green)
{
	_pos = pos;
	_ownerId = ownerId;
	_color = Color::Green;
	_timer.SetTargetTime(_moveTimer);

}

Unit::Unit(Vector2 pos, Color color)
	:super("U", pos, Color::Red)
{
	_pos = pos;
	_color = Color::Red;

}

Unit::~Unit()
{
}

void Unit::Tick(float deltaTime)
{
	super::Tick(deltaTime);
	if (_isMoving)
		move();;

	_timer.Tick(deltaTime);
	
	if (_isDamaged)
	{
		UpdateDamageAnim(deltaTime);
	}
}

void Unit::MoveUnit(std::vector<Vector2> path)
{
	_path = path;
	_pathIndex = 0;
	_isMoving = true;
	_timer.Reset();
}

void Unit::move()
{
	if (_timer.IsTimeOut())
	{
		if (_pathIndex < _path.size())
		{
			SetPosition(_pos);
			_pos = _path[_pathIndex];
			_pathIndex++;
			_timer.Reset();
		}
		else
		{
			_isMoving = false;
		}
	}
}

void Unit::Select()
{
	color = Color::Purple;
	_isSelected = true;
}

void Unit::Unselect()
{
	color = Color::Green;
	_isSelected = false;
}

void Unit::PlayDamageAnim()
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

void Unit::UpdateDamageAnim(float deltaTime)
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

void Unit::StopDamageAnim()
{
	_isDamaged = false;

	ChangeImage("U");
	color = _color;
}
