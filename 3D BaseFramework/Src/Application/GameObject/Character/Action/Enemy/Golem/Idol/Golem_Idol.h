#pragma once
#include"../Golem_ActionState.h"

class Golem_Idol :public Golem_ActionState
{
public:
	Golem_Idol() { Init(); }
	~Golem_Idol()override {};

	void Center()override;

	void Init()  override;

private:

};