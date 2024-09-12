#pragma once
#include"../Enemy_Attack.h"

class Bone_Attack :public Enemy_Attack
{
public:
	Bone_Attack() { Init(); }
	~Bone_Attack()override {};

	void Center()override;

	void Event()override;

private:

};