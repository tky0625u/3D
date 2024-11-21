#pragma once
#include"../GameScene_State.h"

class GameScene_GameOver :public GameScene_State
{
public:
	GameScene_GameOver() {};
	~GameScene_GameOver()override {};

	void Event()         override;
	void ChangeState()   override;

private:

};