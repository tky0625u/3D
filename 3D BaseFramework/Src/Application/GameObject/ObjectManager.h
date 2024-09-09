#pragma once

class Player;
class EnemyBase;

class ObjectManager
{
public:

	void SetObjectParam();
	void SetPlayerParam();
	void SetWeaponParam();
	void SetEnemyParam(std::string _StageNum);

	std::vector<std::shared_ptr<EnemyBase>> GetEnemyList()const { return m_EnemyList; }

private:
	std::weak_ptr<Player> m_player;
	UINT m_id = 0;
	std::vector<std::shared_ptr<EnemyBase>> m_EnemyList;

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