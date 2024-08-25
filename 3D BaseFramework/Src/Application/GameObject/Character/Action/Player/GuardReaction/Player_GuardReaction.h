#pragma once
#include"../../ActionBase.h"

class Player_GuardReaction :public ActionBase
{
public:
	Player_GuardReaction() { Init(); }
	~Player_GuardReaction()override {};

	void Start()override;
	
private:

};