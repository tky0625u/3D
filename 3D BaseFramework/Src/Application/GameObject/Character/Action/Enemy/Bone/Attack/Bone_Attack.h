#pragma once
#include"../Bone_ActionState.h"

class Bone_Attack :public Bone_ActionState
{
public:
	Bone_Attack()           { Init(); }
	~Bone_Attack() override {};

	void Start()    override;
	void Center()   override;
	void End()      override;

	void Attack();
	void MoveForward();

private:
	bool m_atkFlg = false;
};