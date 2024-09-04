#pragma once

#include"../BaseScene/BaseScene.h"

class CharacterBase;

class GameScene : public BaseScene
{
public :

	GameScene() {}
	~GameScene() {}

	void Init() override;

private:

	void Event() override;
	

	std::shared_ptr<CharacterBase> m_player;
};
