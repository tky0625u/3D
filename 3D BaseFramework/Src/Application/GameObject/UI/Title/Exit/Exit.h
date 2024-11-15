#pragma once
#include"../../UIBase.h"

class Exit :public UIBase
{
public:
	Exit() {};
	~Exit()override {}

	void DrawSprite()override;
	void Init()override;

private:

};