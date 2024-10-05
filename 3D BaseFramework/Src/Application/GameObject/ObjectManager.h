#pragma once

class Player;
class EnemyBase;

class ObjectManager:public std::enable_shared_from_this<ObjectManager>
{
public:

	void DeleteEnemyList();
	void SlowChange();

	void SetObjectParam();
	void SetPlayerParam();
	void SetWeaponParam(std::string _filePath, int _id);
	void SetEnemyParam(std::string _StageNum);

	// デバッグ
	void AddBone();

	std::vector<std::weak_ptr<EnemyBase>> GetEnemyList()const { return m_EnemyList; }
	float GetSlow()const { return m_slow; }
	bool GetSlowFlg()const { return m_slowFlg; }

private:
	std::weak_ptr<Player>                 m_player;
	UINT                                  m_id   = 0;
	float                                 m_slow = 1.0f;
	bool                                  m_slowFlg = false;
	std::vector<std::weak_ptr<EnemyBase>> m_EnemyList;

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