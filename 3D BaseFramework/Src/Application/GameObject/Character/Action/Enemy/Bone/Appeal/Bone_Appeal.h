#pragma once
#include"../Bone_ActionState.h"

class Bone_Appeal :public Bone_ActionState
{
public:
	Bone_Appeal() {};
	~Bone_Appeal()override {};

	void Start()override;

	void Hit(std::shared_ptr<Enemy_ConText> context, int _damage)override { return; }

private:
	std::weak_ptr<KdEffekseerObject> m_Effect;
};