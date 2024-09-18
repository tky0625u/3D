#pragma once

#include"../BaseScene/BaseScene.h"

class ObjectManager;

class GameScene : public BaseScene
{
public :

	GameScene() {}
	~GameScene() {}

	void Init() override;

private:

	void Event() override;

	std::shared_ptr<ObjectManager> m_ObjManager = nullptr;
};
