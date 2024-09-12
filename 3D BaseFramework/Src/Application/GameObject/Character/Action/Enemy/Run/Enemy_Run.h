#pragma once
#include"../Enemy_ActionState.h"

class Enemy_Run :public Enemy_ActionState
{
public:
	Enemy_Run() { Init(); }
	~Enemy_Run()   override {};

	virtual void Init()override;

	virtual void Start()    override;
	virtual void Center()   override;
	virtual void End()      override;

	void Chace();
protected:
	bool m_atkFlg = false;
};