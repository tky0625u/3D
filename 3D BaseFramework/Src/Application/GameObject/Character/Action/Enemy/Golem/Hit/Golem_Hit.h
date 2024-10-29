#pragma once
#include"../Golem_ActionState.h"

class Golem_Hit :public Golem_ActionState
{
public:
	Golem_Hit() {}
	~Golem_Hit() override {};

	void Start()override;

	void Hit(int _damage)override;

private:

};