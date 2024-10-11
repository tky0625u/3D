#pragma once
#include"../Bone_ActionState.h"

class Bone_Stumble :public Bone_ActionState
{
public:
	Bone_Stumble()          { Init(); }
	~Bone_Stumble()override {};

	void Start()   override;
	void End()     override;

	void Stumble();

private:

};