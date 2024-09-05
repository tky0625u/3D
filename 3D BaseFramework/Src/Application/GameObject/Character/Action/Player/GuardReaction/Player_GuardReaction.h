#pragma once
#include"../Player_ActionState.h"

class Player_GuardReaction :public Player_ActionState
{
public:
	Player_GuardReaction() { Init(); }
	~Player_GuardReaction()override {};

	void Start()override;

	void Guard(std::shared_ptr<Player_ActionConText> context)         override;              //ガード

private:

};