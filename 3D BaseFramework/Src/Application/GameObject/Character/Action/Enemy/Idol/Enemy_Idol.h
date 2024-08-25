#pragma once
#include"../../ActionBase.h"

class Enemy_Idol :public ActionBase
{
public:
	Enemy_Idol() {
		m_ChangeFlg = true;
		m_flow = Flow::CenterType;
		m_end = false;
	}
	~Enemy_Idol()   override {};

	void Center()   override;
	void Reset();

private:
};