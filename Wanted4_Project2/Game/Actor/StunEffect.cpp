#include "StunEffect.h"

static const StunEffect::EffectFrame sequence[] =
{
	StunEffect::EffectFrame("t", 0.1f, Color::Red),
	StunEffect::EffectFrame("T", 0.1f, Color::Blue),
	StunEffect::EffectFrame("T", 0.1f, Color::Green),
	StunEffect::EffectFrame("t", 0.1f, Color::Red),
	StunEffect::EffectFrame("t", 0.1f, Color::Purple),
	
};

StunEffect::StunEffect(const Vector2& position)
{
	sortingOrder = 10;
	int effectFrameImageLemgth = 1;

	// 애니메이션 시퀀스 개수 구하기.
	effectSequenceCount = sizeof(sequence) / sizeof(sequence[0]);

	// 다음 애니메이션까지 대기할 시간.
	timer.SetTargetTime(sequence[0].playTime);

	// 색상 설정.
	color = sequence[0].color;
}

void StunEffect::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 애니메이션 재생을 위한 타이머 업데이트.
	timer.Tick(deltaTime);
	if (!timer.IsTimeOut())
	{
		return;
	}

	// 애니메이션 재생 끝났는지 확인.
	// 끝났으면 처음부터 다시
	if (currentSequenceIndex == effectSequenceCount - 1)
	{
		//Destroy();
		currentSequenceIndex = 0;
		return;
	}

	// 타이머 리셋.
	timer.Reset();

	// 이펙트 프레임 업데이트.
	++currentSequenceIndex;

	// 다음 시퀀스에서 재생할 시간으로 타이머 재설정.
	timer.SetTargetTime(sequence[currentSequenceIndex].playTime);

	// 애니메이션 프레임에 사용할 문자열을 액터에 복사.
	ChangeImage(sequence[currentSequenceIndex].frame);

	// 색상 설정.
	color = sequence[currentSequenceIndex].color;
}
