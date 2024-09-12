#pragma once
#include"../Enemy_ActionState.h"

class Enemy_Attack :public Enemy_ActionState
{
public:
	Enemy_Attack() { Init(); }
	~Enemy_Attack()   override {};

	virtual void Start()    override;
	virtual void Center()   override;
	virtual void End()      override;

	void Attack();

protected:
	bool m_atkFlg = false;
};