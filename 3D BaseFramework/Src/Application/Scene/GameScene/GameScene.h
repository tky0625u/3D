#pragma once

#include"../BaseScene/BaseScene.h"

class ObjectManager;
class StageManager;

class GameScene : public BaseScene
{
public :

	GameScene() {}
	~GameScene() {}

	void Update()    override;
	void PostUpdate()override;
	void Init()      override;

	void SetObjManager(std::shared_ptr<ObjectManager> _ObjManager) { m_ObjManager = _ObjManager; }

private:

	void Event() override;

	std::shared_ptr<StageManager> m_StageManager = nullptr;
};
