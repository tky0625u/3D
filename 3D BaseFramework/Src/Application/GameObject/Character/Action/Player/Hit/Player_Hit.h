#pragma once
#include"../Player_ActionState.h"

class Player_Hit :public Player_ActionState
{
public:
	Player_Hit() { Init(); }
	~Player_Hit()override {};

	void Start()override;

	void Idol(std::shared_ptr<Player_ActionConText> context)          override;             //待ち

private:

};