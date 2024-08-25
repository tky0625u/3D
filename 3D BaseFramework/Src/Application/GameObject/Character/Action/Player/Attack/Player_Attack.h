#pragma once
#include"../../ActionBase.h"

class CharacterBase;

class Player_Attack :public ActionBase
{
public:
	Player_Attack() { Init(); }
	~Player_Attack()   override {};

	void Center()   override;
	void End()      override;

	void KeyCheck(const UINT key, const UINT before) override;
	void Init()     override;
	void Reset()override;

private:
	static const int AttackNUM = 3;
	int m_atkNum = 1;
};