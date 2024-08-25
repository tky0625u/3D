#pragma once
#include"../../ActionBase.h"

class Player_Idol :public ActionBase
{
public:
	Player_Idol() 	{ 
						m_ChangeFlg = true;
						m_flow = Flow::CenterType;
						m_end = false;
					}
	~Player_Idol()   override {};

	void Center()   override;
	void Reset()    override;

	void KeyCheck(const UINT key,const UINT before) override;

private:

};