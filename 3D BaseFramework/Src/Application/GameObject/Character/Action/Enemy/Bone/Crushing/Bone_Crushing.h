#pragma once
#include"../Bone_ActionState.h"

class Bone_Crushing :public Bone_ActionState
{
public:
	Bone_Crushing() { Init(); }
	~Bone_Crushing()override {};

	void Start()   override;

	void CrushingAction();

	void Run()override            { return; }
	void Attack()override         { return; }
	void Hit(int _damage)override { return; }

private:

};