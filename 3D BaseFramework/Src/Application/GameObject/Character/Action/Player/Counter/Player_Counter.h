#pragma once
#include"../../ActionBase.h"

class Player_Counter :public ActionBase
{
public:
	Player_Counter() { Init(); }
	~Player_Counter()override {};

	void Start()override;
	void End()override;

	void KeyCheck(const UINT key, const UINT before) override;

private:

};