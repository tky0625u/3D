#pragma once

#include"../BaseScene/BaseScene.h"

class TitleScene : public BaseScene
{
public :

	TitleScene() {}
	~TitleScene() {}

	void Update()override;
	void DebugObject()override;

private :

	void Event() override;
	void Init() override;
};
