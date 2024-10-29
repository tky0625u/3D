#pragma once
#include"../Player_ActionState.h"

class EnemyBase;

class Player_GuardReaction :public Player_ActionState
{
public:
	Player_GuardReaction()          { Init(); }
	~Player_GuardReaction()override {};

	void Start()           override;

	void Init()            override;

	void Guard()override;
	void Hit(int _damage = 0, std::shared_ptr<EnemyBase> _enemy = nullptr)override { return; }

private:
	int m_guardTime = 30;
};