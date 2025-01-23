#pragma once

// UI基底
#include"../../UIBase.h"

class Exit :public UIBase
{
public:
	Exit() {};
	~Exit()override {}

	void DrawSprite()override;
	void Init()override;

	// ゲッター================================================================
	const float& GetHeight()const; // 縦幅
	const float& GetWidth()const;  // 横幅
	//=========================================================================

private:

};