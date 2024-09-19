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

	void Guard(std::shared_ptr<Player_ActionConText> context)override;
	void Hit(std::shared_ptr<Player_ActionConText> context, int _damage = 0, std::shared_ptr<EnemyBase> _enemy = nullptr)override { return; }

private:

};