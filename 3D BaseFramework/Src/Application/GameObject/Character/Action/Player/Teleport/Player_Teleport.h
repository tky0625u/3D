#pragma once
#include"../Player_ActionState.h"

class Player_Teleport :public Player_ActionState
{
public:
	Player_Teleport()          {};
	~Player_Teleport()override {};

	void Start() override;
	void Center()override;
	void End()   override;

private:
	Effekseer::Handle m_handle;
};