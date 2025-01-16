#pragma once

// シーン基底
#include"../BaseScene/BaseScene.h"

class TitleScene : public BaseScene
{
public :

	TitleScene() {}
	~TitleScene() {}

	// デバッグウィンドウ
	void DebugObject()override;

private :

	void Event() override;
	void Init() override;
};
