#pragma once
#include"../../UIBase.h"

class TitleGuide :public UIBase
{
public:
	TitleGuide() {};
	~TitleGuide()override {};

	void DrawSprite()override;
	void Init()override;

private:

};