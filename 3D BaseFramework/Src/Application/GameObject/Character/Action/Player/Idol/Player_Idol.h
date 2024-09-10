#pragma once
#include"../Player_ActionState.h"

class CameraBase;
class EnemyBase;

class Player_Idol :public Player_ActionState
{
public:
	Player_Idol() 	{ 
						m_flow = Flow::CenterType;
					}
	~Player_Idol()   override {};

	void Center()   override;

	void ChangeAction()override;

private:

};