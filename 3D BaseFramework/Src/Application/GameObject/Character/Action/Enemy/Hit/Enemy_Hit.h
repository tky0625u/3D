#pragma once
#include"../Enemy_ActionState.h"

class Enemy_Hit :public Enemy_ActionState
{
public:
	Enemy_Hit() { Init(); }
	~Enemy_Hit()override {};

	virtual void Start()override;

	void Hit(std::shared_ptr<Enemy_ConText> context, int _damage)override { return; }

protected:

};