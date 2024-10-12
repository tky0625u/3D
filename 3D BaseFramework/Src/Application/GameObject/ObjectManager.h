#pragma once

class Player;
class EnemyBase;
class Bone;
#include"../tinygltf/json.hpp"
class ObjectManager:public std::enable_shared_from_this<ObjectManager>
{
public:

	void DeleteEnemyList();
	void StopTimeMinus() { if (m_stopTime > 0)m_stopTime--; }
	void SlowChange();
	bool IsWaveMax();

	void PlayerWrite();
	void EnemyWrite(int _wave);

	void SetObjectParam();
	void SetPlayerParam();
	void SetWeaponParam(std::string _filePath, std::string _weaponName);
	void SetEnemyParam(std::string _filePath = "none");
	void SetStopTime(int _stopTime) { m_stopTime = _stopTime; }

	// デバッグ
	void AddBone();
	void AddWeapon(std::string _filePath,std::string _weaponName);
	void ChangeWeapon(std::string _swordName, std::string _shieldName);
	const std::vector<std::string> GetSwordNameList() const { return m_swordNameList; }
	const std::vector<std::string> GetShieldNameList() const { return m_shieldNameList; }

	std::weak_ptr<Player> GetPlayer()const { return m_player; }
	std::vector<std::weak_ptr<EnemyBase>> GetEnemyList()const { return m_EnemyList; }
	std::vector<std::weak_ptr<Bone>> GetBoneList()const       { return m_BoneList; }
	int GetStopTime()const { return m_stopTime; }
	float GetSlow()const { return m_slow; }
	bool GetSlowFlg()const { return m_slowFlg; }

	// デバッグ
	int GetMaxWave()const { return m_MaxWave; }
	int GetnowWave()const { return m_nowWave; }

private:
	std::weak_ptr<Player>                 m_player;
	UINT                                  m_id       = 0;
	int                                   m_stopTime = 0;
	float                                 m_slow     = 1.0f;
	bool                                  m_slowFlg  = false;
	std::vector<std::weak_ptr<EnemyBase>> m_EnemyList;
	std::vector<std::weak_ptr<Bone>>      m_BoneList;
	nlohmann::json                        m_EnemyJson;
	int                                   m_MaxWave  = 0;
	int                                   m_nowWave  = 0;

	//デバッグ
	std::vector<std::string>              m_swordNameList;
	std::vector<std::string>              m_shieldNameList;

private:
	ObjectManager() {};
	~ObjectManager() {};

public:
	static ObjectManager& Instance()
	{
		static ObjectManager instance;
		return instance;
	}
};