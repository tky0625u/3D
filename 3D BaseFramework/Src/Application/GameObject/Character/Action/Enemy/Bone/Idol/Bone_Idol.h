#pragma once
#include"../Bone_ActionState.h"

class Bone_Idol :public Bone_ActionState
{
public:
	Bone_Idol()           { Init(); }
	~Bone_Idol() override {};

	void Center()override;

	void Init()  override;

private:

};