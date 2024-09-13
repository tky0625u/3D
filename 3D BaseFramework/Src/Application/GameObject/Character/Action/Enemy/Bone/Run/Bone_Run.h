#pragma once
#include"../Bone_ActionState.h"

class Bone_Run :public Bone_ActionState
{
public:
	Bone_Run()               { Init(); }
	~Bone_Run()     override {};
	
	void Start()    override;
	void Center()   override;
	void End()      override;

	void Chace();

private:
	bool m_atkFlg = false;
};