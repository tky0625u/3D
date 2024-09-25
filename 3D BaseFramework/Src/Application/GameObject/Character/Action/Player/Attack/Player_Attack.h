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
	void Attack4();
	void AttackDirCheck();
	void ChangeAction()override;
	bool AttackRangeCheck();

private:
	Math::Vector3    m_AttackDir = Math::Vector3::Zero;
	static const int AttackNUM = 4;
	int              m_atkNum = 1;
};