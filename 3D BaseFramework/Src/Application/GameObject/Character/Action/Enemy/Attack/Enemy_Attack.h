#pragma once
#include"../../ActionBase.h"

class CharacterBase;

class Enemy_Attack :public ActionBase
{
public:
	Enemy_Attack() { Init(); }
	~Enemy_Attack()   override {};

	void Start()    override;
	void Center()   override;
	void End()      override;

private:
};