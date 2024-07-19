#pragma once
#include"../CharacterBase.h"

class LeaderBase;

class SoldierBase :public CharacterBase
{
public:
	SoldierBase() {}
	~SoldierBase()override {};

	void Action()override;
	void Init()  override;

	void SetEnemyLeader(std::shared_ptr<LeaderBase> _enemyLeader) { m_enemyLeader = _enemyLeader; }

protected:
	CharacterType m_enemyType;
	std::weak_ptr<LeaderBase> m_enemyLeader;
};