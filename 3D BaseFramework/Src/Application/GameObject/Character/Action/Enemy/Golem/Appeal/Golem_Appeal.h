#pragma once
#include"../Golem_ActionState.h"

class Golem_Appeal :public Golem_ActionState
{
public:
	Golem_Appeal() {};
	~Golem_Appeal()override {};

	void Start()override;

	void Hit(std::shared_ptr<Enemy_ConText> context, int _damage)override { return; }

private:
	std::weak_ptr<KdEffekseerObject> m_Effect;
};