#pragma once
#include"../../Golem_ActionState.h"

class Golem_Attack3 :public Golem_ActionState
{
public:
	Golem_Attack3 ()          { m_pDebugWire = std::make_unique<KdDebugWireFrame>(); }
	~Golem_Attack3() override {};

	void Start()    override;
	void Center()   override;
	void End()      override;

	void AttackDamage();

	void Hit(int _damage)override;
	void Stumble()       override { return; }

private:
};