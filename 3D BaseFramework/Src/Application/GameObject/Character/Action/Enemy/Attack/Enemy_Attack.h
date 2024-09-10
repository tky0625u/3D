#pragma once
#include"../Enemy_ActionState.h"

class Enemy_Attack :public Enemy_ActionState
{
public:
	Enemy_Attack() { Init(); }
	~Enemy_Attack()   override {};

	void Start()    override;
	void Center()   override;
	void End()      override;

	void Event()override;

private:
	bool m_atkFlg = false;
};