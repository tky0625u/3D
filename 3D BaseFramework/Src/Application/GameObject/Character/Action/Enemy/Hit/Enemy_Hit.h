#pragma once
#include"../../ActionBase.h"

class Enemy_Hit :public ActionBase
{
public:
	Enemy_Hit() { Init(); }
	~Enemy_Hit()override {};

	void Start()override;

private:

};