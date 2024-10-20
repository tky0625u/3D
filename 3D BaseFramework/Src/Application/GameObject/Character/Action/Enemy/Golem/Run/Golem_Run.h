#pragma once
#include"../Golem_ActionState.h"

class Golem_Run :public Golem_ActionState
{
public:
	Golem_Run()               {}
	~Golem_Run()     override {};

	void Start()    override;
	void Center()   override;
	void End()      override;

	void Chace();

private:

};