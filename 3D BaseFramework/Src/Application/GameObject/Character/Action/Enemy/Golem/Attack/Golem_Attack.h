#pragma once
#include"../Golem_ActionState.h"

class Golem_Attack :public Golem_ActionState
{
public:
	Golem_Attack()           {}
	~Golem_Attack() override {};

	void Start()    override;
	void Center()   override;
	void End()      override;

	void Attack();

private:
	bool m_atkFlg = false;
};