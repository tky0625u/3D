#pragma once
#include"../Bone_ActionState.h"

class Bone_Hit :public Bone_ActionState
{
public:
	Bone_Hit()           { Init(); }
	~Bone_Hit() override {};

	void Start()override;

	void Run()override { return; }
	void Attack()override { return; }
	void Stumble()override { return; }
	void Hit(int _damage)override;

private:

};