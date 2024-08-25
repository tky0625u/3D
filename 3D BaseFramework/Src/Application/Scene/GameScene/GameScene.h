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
	void MapLoad(std::string _filePath);
	void Load(int StageNumber,int Z,int X);

	static const int                              m_ObjDistans   = 64;
	const float                                   m_StageDistans = 50.0f;
	std::weak_ptr<CharacterBase>                  m_player;

};
