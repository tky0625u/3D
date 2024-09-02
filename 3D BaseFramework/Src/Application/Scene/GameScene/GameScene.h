#pragma once

#include"../BaseScene/BaseScene.h"

class CharacterBase;

class GameScene : public BaseScene
{
public :
	enum ObjectType
	{
		FloorType,
		PlayerType
	};

	GameScene() { Init(); }
	~GameScene() {}

private:

	void Event() override;
	void Init() override;

	void RegisterObj(std::string_view ObjectName, std::shared_ptr<KdGameObject> Object);
	std::unordered_map<std::string, std::shared_ptr<KdGameObject>> m_ObjectList;
	std::weak_ptr<CharacterBase>                  m_player;

};
