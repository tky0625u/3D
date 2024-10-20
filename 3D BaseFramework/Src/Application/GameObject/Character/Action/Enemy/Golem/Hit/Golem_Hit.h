#pragma once
#include"../Golem_ActionState.h"

class Golem_Hit :public Golem_ActionState
{
public:
	Golem_Hit() {}
	~Golem_Hit() override {};

	void Start()override;

	void Hit(std::shared_ptr<Enemy_ConText> context, int _damage)override;

private:

};