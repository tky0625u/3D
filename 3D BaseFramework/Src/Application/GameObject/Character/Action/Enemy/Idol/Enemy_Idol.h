#pragma once
#include"../Enemy_ActionState.h"

class Enemy_Idol :public Enemy_ActionState
{
public:
	Enemy_Idol() {
		m_flow = Flow::CenterType;
	}
	~Enemy_Idol()   override {};

	void Center()   override;
	void Event()    override;

private:
};