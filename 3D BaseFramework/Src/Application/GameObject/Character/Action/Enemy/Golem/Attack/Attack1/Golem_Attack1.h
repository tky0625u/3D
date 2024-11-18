#pragma once
#include"../../Golem_ActionState.h"

class Golem_Attack1 :public Golem_ActionState
{
public:
	Golem_Attack1() {}
	~Golem_Attack1() override {};

	void Start()    override;
	void Center()   override;
	void End()      override;

	void Hit(int _damage)override;

private:
	bool m_atkFlg = false;
};