#pragma once
#include"../Player_ActionState.h"

class CameraBase;
class EnemyBase;

class Player_Counter :public Player_ActionState
{
public:
	Player_Counter() { Init(); }
	~Player_Counter()override {};

	void Start()override;
	void Center()override;
	void End()override;

	void Event() override;

	void ChangeAction()override;

	void SetTargetPos(Math::Vector3 targetPos) { m_targetPos = targetPos; }

	void Hit(std::shared_ptr<Player_ActionConText> context, int _damage = 0, std::shared_ptr<EnemyBase> _enemy = nullptr)override { return; }

private:
	Math::Vector3 m_targetPos = Math::Vector3::Zero;
};