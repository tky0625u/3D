#pragma once
#include"../../ActionBase.h"

class Player_Hit :public ActionBase
{
public:
	Player_Hit() { Init(); }
	~Player_Hit()override {};

	void Start()override;

private:

};