#pragma once
#include"../Golem_ActionState.h"

class Golem_Crushing :public Golem_ActionState
{
public :
	Golem_Crushing()          {};
	~Golem_Crushing()override {};

	void Start()override;

	void CrushingAction();

	void Run()override { return; }
	void Attack()override { return; }
	void Hit(int _damage)override { return; }

private:

};