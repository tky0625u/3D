#pragma once
#include"../Enemy_ActionState.h"

class Enemy_Run :public Enemy_ActionState
{
public:
	Enemy_Run() { Init(); }
	~Enemy_Run()   override {};

	void Init()override;

	void Start()    override;
	void Center()   override;
	void End()      override;

	void Event()override;
private:
	bool m_atkFlg = false;
};