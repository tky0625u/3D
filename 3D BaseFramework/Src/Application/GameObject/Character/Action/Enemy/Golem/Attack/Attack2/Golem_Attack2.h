#pragma once
#include"../../Golem_ActionState.h"

class Golem_Attack2 :public Golem_ActionState
{
public:
	Golem_Attack2() { m_pDebugWire = std::make_unique<KdDebugWireFrame>(); }
	~Golem_Attack2() override {};

	void Start()    override;
	void Center()   override;
	void End()      override;

	void AttackDamage();

	void Hit(int _damage)override;
	void Stumble()       override { return; }

private:
	Math::Vector3 m_playerPos = Math::Vector3::Zero;
	bool m_atkFlg = false;
};