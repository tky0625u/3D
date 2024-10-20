#pragma once
#include"../../UIBase.h"

class LockON :public UIBase
{
public:
	LockON() {};
	~LockON()override {};

	void Update()override;
	void DrawSprite()override;
	void Init()override;

private:

};