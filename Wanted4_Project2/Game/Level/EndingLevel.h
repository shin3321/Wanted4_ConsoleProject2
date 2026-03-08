#pragma once
#include "Level\Level.h"
#include "Utils/Timer.h"

using namespace Wanted;

class EndingLevel :   public Wanted::Level
{
	RTTI_DECLARATIONS(EndingLevel, Level)
public:
	EndingLevel() {}
	EndingLevel(int victory);

	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

private:
	int _vic;
	Timer _timer;
};

