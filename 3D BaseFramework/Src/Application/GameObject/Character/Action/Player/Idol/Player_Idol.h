#pragma once
#include"../Player_ActionState.h"

class CameraBase;
class EnemyBase;

class Player_Idol :public Player_ActionState
{
public:
	Player_Idol()               { Init(); }
	~Player_Idol()     override {};

	void Center()      override;

	void Init()        override;

	void ChangeAction()override;

private:

};