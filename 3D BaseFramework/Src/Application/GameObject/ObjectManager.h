#pragma once

class Player;
class EnemyBase;
class Bone;
class GameCamera;
class Title;
class Game;
class Exit;
class Cursor;
class TitleGuide;
class TitleCamera;
class Ground;
class Circle;
class MagicPolygon;

#include"../tinygltf/json.hpp"
class ObjectManager :public std::enable_shared_from_this<ObjectManager>
{
public:
	ObjectManager() {};
	~ObjectManager() {};

	void SceneCheck();
	void DeleteEnemyList();
	void DeleteObjectList();
	void SlowChange();
	void NextStageLiberation();
	void GameClear();
	void CreateStage();

	void DebugObject();

	void TitleCameraWrite();
	void TitleWrite();
	void GameWrite();
	void ExitWrite();
	void TitleGuideWrite();
	void CursorWrite();
	void GameCameraWrite();
	void PlayerWrite(std::string _fileName);
	void EnemyWrite(int _stage, int _wave, std::string _fileName);
	void SwordWrite(std::string _swordName, std::string _fileName);
	void ShieldWrite(std::string _shieldName, std::string _fileName);
	void ObjectWrite(std::string _fileName);

	void SetTitleCamera();
	void SetTitleParam();
	void SetGameParam();
	void SetExitParam();
	void SetTitleGuideParam();
	void SetCursorParam();
	void SetGameCameraParam();
	void SetObjectParam();
	void SetPlayerParam();
	void SetWeaponParam(std::string _filePath, std::string _weaponName);
	void SetEnemyParam(std::string _filePath = "none");

	// デバッグ
	void AddTitleCamera();
	void AddTitle();
	void AddGame();
	void AddExit();
	void AddTitleGuide();
	void AddCursor();
	void AddBone();
	void AddGolem();
	void AddWeapon(std::string _filePath, std::string _weaponName);
	void AddGround();
	void AddCircle();
	void AddWall();
	void AddSkyBox();
	void ChangeWeapon(std::string _swordName, std::string _shieldName);

	const float GetSlow()const { return m_slow; }
	const bool GetSlowFlg()const { return m_slowFlg; }

private:
	std::weak_ptr<TitleCamera>               m_titleCamera;
	std::weak_ptr<Title>                     m_title;
	std::weak_ptr<Game>                      m_game;
	std::weak_ptr<Exit>                      m_exit;
	std::weak_ptr<Cursor>                    m_cursor;
	std::weak_ptr<TitleGuide>                m_titleGuide;
	std::weak_ptr<GameCamera>                m_camera;
	std::weak_ptr<Player>                    m_player;
	std::weak_ptr<Ground>                    m_ground;
	std::weak_ptr<Circle>                    m_circle;
	std::weak_ptr<MagicPolygon>              m_magic;
	UINT                                     m_id = 0;
	float                                    m_slow = 1.0f;
	bool                                     m_slowFlg = false;
	std::vector<std::weak_ptr<EnemyBase>>    m_EnemyList;
	std::vector<std::weak_ptr<KdGameObject>> m_ObjectList;
	nlohmann::json                           m_EnemyJson;

	//デバッグ							     
	std::vector<std::string>                 m_swordNameList;
	std::vector<std::string>                 m_shieldNameList;

	std::string                              m_nowScene;

};
