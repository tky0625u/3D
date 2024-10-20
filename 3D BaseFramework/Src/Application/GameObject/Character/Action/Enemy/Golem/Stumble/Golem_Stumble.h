#pragma once
#include"../Golem_ActionState.h"

class Golem_Stumble :public Golem_ActionState
{
public:
	Golem_Stumble() {}
	~Golem_Stumble()override {};

	void Start()   override;
	void End()     override;

	void Stumble();

private:

};