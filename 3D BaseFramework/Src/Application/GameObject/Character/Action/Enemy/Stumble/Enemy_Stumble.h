#pragma once
#include"../Enemy_ActionState.h"

class Enemy_Stumble :public Enemy_ActionState
{
public:
	Enemy_Stumble() { Init(); }
	~Enemy_Stumble()override {};

	void Start()override;
	void End()override;

private:

};