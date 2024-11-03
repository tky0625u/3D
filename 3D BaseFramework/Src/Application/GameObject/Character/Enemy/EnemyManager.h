#pragma once

class EnemyBase;

class EnemyManager
{
public:
	EnemyManager() {};
	~EnemyManager() {};

	void DeleteEnemyList();

	void SetEnemyList(std::vector<std::weak_ptr<EnemyBase>> _EnemyList) { m_EnemyList; }

private:
	std::vector<std::weak_ptr<EnemyBase>> m_EnemyList;
};