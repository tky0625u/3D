#pragma once
#include"../GameScene_State.h"

class GameScene_Normal :public GameScene_State
{
public:
	GameScene_Normal()          {};
	~GameScene_Normal()override {};

	void Event()      override;
	void ChangeState()override;

private:

};