#pragma once
#include"../Player_ActionState.h"

class CharacterBase;
class EnemyBase;

class Player_Guard :public Player_ActionState
{
public:
	Player_Guard()           {}
	~Player_Guard() override {};

	void Start()    override;
	void Center()   override;
	void End()      override;

	void GuardRotate(Math::Vector3 _pos);

	void SetGuardTime(int _guardTime) { m_guardTime = _guardTime; }

	void Hit(int _damage = 0, std::shared_ptr<EnemyBase> _enemy = nullptr)override; //被弾

private:
	int m_guardTime = 0;
};