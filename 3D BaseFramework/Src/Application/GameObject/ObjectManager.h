#pragma once

class Player;
class EnemyBase;

class ObjectManager
{
public:

	void DeleteEnemyList();

	void SetObjectParam();
	void SetPlayerParam();
	void SetWeaponParam(std::string _filePath, int _id);
	void SetEnemyParam(std::string _StageNum);

	std::vector<std::weak_ptr<EnemyBase>> GetEnemyList()const { return m_EnemyList; }

private:
	std::weak_ptr<Player> m_player;
	UINT m_id = 0;
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