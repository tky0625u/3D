#pragma once
#include"../Player_ActionState.h"

class CameraBase;
class EnemyBase;

class Player_Attack :public Player_ActionState
{
public:
	Player_Attack()             { Init(); }
	~Player_Attack()   override {};

	void Center()      override;
	void End()         override;

	void ChangeAction()override;
	void Init()        override;

private:
	static const int AttackNUM = 3;
	int m_atkNum = 1;
};