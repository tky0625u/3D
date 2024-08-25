#pragma once
#include"../../ActionBase.h"

class CharacterBase;

class Enemy_Run :public ActionBase
{
public:
	Enemy_Run() { Init(); }
	~Enemy_Run()   override {};

	void Start()    override;
	void Center()   override;
	void End()      override;

	void Chase();

private:
	bool m_atkFlg = false;
};