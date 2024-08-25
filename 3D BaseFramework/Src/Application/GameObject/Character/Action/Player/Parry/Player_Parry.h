#pragma once
#include"../../ActionBase.h"

class Player_Parry :public ActionBase
{
public:
	Player_Parry() { Init(); }
	~Player_Parry()override {};

	void Start()override;
	void End()override;
	void Reset();

	void KeyCheck(const UINT key, const UINT before) override;

private:

};