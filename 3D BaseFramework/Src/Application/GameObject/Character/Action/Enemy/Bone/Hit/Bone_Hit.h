#pragma once
#include"../Bone_ActionState.h"

class Bone_Hit :public Bone_ActionState
{
public:
	Bone_Hit()           { Init(); }
	~Bone_Hit() override {};

	void Start()override;

	void Hit(std::shared_ptr<Enemy_ConText> context, int _damage)override { return; }

private:

};