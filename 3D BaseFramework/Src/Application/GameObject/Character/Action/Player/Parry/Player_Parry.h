#pragma once
#include"../Player_ActionState.h"

class CameraBase;
class EnemyBase;

class Player_Parry :public Player_ActionState
{
public:
	Player_Parry()              {}
	~Player_Parry()    override {};

	void Start()       override;
	void End()         override;

	void ChangeAction()override;

	void Hit(int _damage = 0, std::shared_ptr<EnemyBase> _enemy = nullptr)override { return; }

private:

};