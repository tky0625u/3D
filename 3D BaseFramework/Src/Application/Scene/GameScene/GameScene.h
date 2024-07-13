#pragma once

#include"../BaseScene/BaseScene.h"

class CharacterBase;

class GameScene : public BaseScene
{
public :
	enum ObjectType
	{
		FloorType,
		WallType,
		PlayerType
	};

	GameScene() { Init(); }
	~GameScene() {}

private:

	void Event() override;
	void Init() override;
	void Load(int StageNumber);

	static const int                              m_ObjDistans = 4;
	std::weak_ptr<CharacterBase>                  m_player;
	std::shared_ptr<KdModelData>                  m_floorModel;
	std::shared_ptr<KdModelData>                  m_wallModel;
};
