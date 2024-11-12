#pragma once

#include"../BaseScene/BaseScene.h"

class GameScene : public BaseScene
{
public :

	GameScene() {}
	~GameScene() {}

	void Update()    override;
	void PostUpdate()override;
	void Init()      override;

	void DebugObject()override;

private:

	void Event() override;

};
