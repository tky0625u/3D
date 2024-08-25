#pragma once
#include"../../ActionBase.h"

class Enemy_Stumble :public ActionBase
{
public:
	Enemy_Stumble() { Init(); }
	~Enemy_Stumble()override {};

	void Start()override;
	void End()override;

	void Stumble();

private:

};