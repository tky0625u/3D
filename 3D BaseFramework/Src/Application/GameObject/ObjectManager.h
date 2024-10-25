#pragma once

class Player;
class EnemyBase;
class Bone;
class TPSCamera;
class Title;
class Game;
class Exit;
class Cursor;

#include"../tinygltf/json.hpp"
class ObjectManager:public std::enable_shared_from_this<ObjectManager>
{
public:
	ObjectManager() {};
	~ObjectManager() {};

	void SceneCheck();
	void DeleteEnemyList();
	void DeleteObjectList();
	void StopTimeMinus() { if (m_stopTime > 0)m_stopTime--; }
	void SlowChange();
	void Clear();
	bool IsWaveMax();

	void TitleWrite();
	void GameWrite();
	void ExitWrite();
	void CursorWrite();
	void GameCameraWrite();
	void PlayerWrite(std::string _fileName);
	void EnemyWrite(int _stage, int _wave, std::string _fileName);
	void SwordWrite(std::string _swordName, std::string _fileName);
	void ShieldWrite(std::string _shieldName, std::string _fileName);
	void ObjectWrite(std::string _fileName);

	void SetTitleParam();
	void SetGameParam();
	void SetExitParam();
	void SetCursorParam();
	void SetGameCameraParam();
	void SetObjectParam();
	void SetPlayerParam();
	void SetWeaponParam(std::string _filePath, std::string _weaponName);
	void SetEnemyParam(std::string _filePath = "none");
	void SetStopTime(int _stopTime) { m_stopTime = _stopTime; }

	// デバッグ
	void AddTitle();
	void AddGame();
	void AddExit();
	void AddCursor();
	void AddBone();
	void AddGolem();
	void AddWeapon(std::string _filePath,std::string _weaponName);
	void AddGround();
	void AddCircle();
	void AddWall();
	void ChangeWeapon(std::string _swordName, std::string _shieldName);
	const std::vector<std::string> GetSwordNameList() const { return m_swordNameList; }
	const std::vector<std::string> GetShieldNameList() const { return m_shieldNameList; }

	const std::weak_ptr<Title> GetTitle()const { return m_title; }
	const std::weak_ptr<Game> GetGame()const { return m_game; }
	const std::weak_ptr<Exit> GetExit()const { return m_exit; }
	const std::weak_ptr<Cursor> GetCursor()const { return m_cursor; }
	const std::weak_ptr<TPSCamera> GetCamera()const { return m_camera; }
	const std::weak_ptr<Player> GetPlayer()const { return m_player; }
	const std::vector<std::weak_ptr<EnemyBase>> GetEnemyList()const { return m_EnemyList; }
	const std::vector<std::weak_ptr<KdGameObject>> GetObjectList()const{ return m_ObjectList; }
	const int GetStopTime()const { return m_stopTime; }
	const float GetSlow()const { return m_slow; }
	const bool GetSlowFlg()const { return m_slowFlg; }
	const int GetnowStage()const { return m_nowStage; }

	// デバッグ
	int GetMaxWave()const { return m_MaxWave; }
	int GetnowWave()const { return m_nowWave; }

private:
	std::weak_ptr<Title>                     m_title;
	std::weak_ptr<Game>                      m_game;
	std::weak_ptr<Exit>                      m_exit;
	std::weak_ptr<Cursor>                      m_cursor;
	std::weak_ptr<TPSCamera>                 m_camera;
	std::weak_ptr<Player>                    m_player;
	UINT                                     m_id       = 0;
	int                                      m_stopTime = 0;
	float                                    m_slow     = 1.0f;
	bool                                     m_slowFlg  = false;
	std::vector<std::weak_ptr<EnemyBase>>    m_EnemyList;
	std::vector<std::weak_ptr<KdGameObject>> m_ObjectList;
	std::vector<Math::Vector3>               m_StartPosList;
	int                                      m_MaxStage = 3;
	int                                      m_nowStage = 1;
										     
	nlohmann::json                           m_EnemyJson;
	int                                      m_MaxWave  = 0;
	int                                      m_nowWave  = 0;
										     
	//デバッグ							     
	std::vector<std::string>                 m_swordNameList;
	std::vector<std::string>                 m_shieldNameList;
										     
	std::string                              m_nowScene;

};