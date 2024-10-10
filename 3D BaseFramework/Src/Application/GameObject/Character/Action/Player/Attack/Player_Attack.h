#pragma once
#include"../Player_ActionState.h"

class CameraBase;
class EnemyBase;

class Player_Attack :public Player_ActionState
{
public:
	Player_Attack()             {}
	~Player_Attack()   override {};

	void Start()       override;
	void Center()      override;
	void End()         override;

	void Attack1();
	void Attack2();
	void Attack3();
	void AttackDirCheck();
	void ChangeAction()override;

private:
	Math::Vector3    m_AttackDir = Math::Vector3::Zero;
	static const int AttackNUM = 3;
	int              m_atkNum = 1;
};