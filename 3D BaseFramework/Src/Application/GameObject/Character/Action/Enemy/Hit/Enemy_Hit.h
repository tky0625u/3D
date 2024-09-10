#pragma once
#include"../Enemy_ActionState.h"

class Enemy_Hit :public Enemy_ActionState
{
public:
	Enemy_Hit() { Init(); }
	~Enemy_Hit()override {};

	void Start()override;

private:

};