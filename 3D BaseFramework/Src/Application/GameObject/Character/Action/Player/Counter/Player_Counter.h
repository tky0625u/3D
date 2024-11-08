#pragma once
#include"../Player_ActionState.h"

class CameraBase;
class EnemyBase;

class Player_Counter :public Player_ActionState
{
public:
	Player_Counter()            {}
	~Player_Counter()  override {};

	void Start()       override;
	void Center()      override;
	void End()         override;

	void ChangeAction()override;

	void SetTargetPos(Math::Vector3 targetPos) { m_targetPos = targetPos; }

	void Hit(int _damage = 0, std::shared_ptr<EnemyBase> _enemy = nullptr)override { return; }

private:
	Math::Vector3 m_targetPos = Math::Vector3::Zero;
};