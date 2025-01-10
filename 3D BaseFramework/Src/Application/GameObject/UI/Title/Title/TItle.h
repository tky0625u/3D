#pragma once

// UI基底
#include"../../UIBase.h"

class Title :public UIBase
{
public:
	Title() {};
	~Title()override {}

	void DrawSprite()override;
	void Init()override;

private:

};