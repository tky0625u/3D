﻿#pragma once
#include"../WeaponBase.h"

class Sword :public WeaponBase
{
public:
	Sword() {}
	~Sword()override {};

	void Update()override;

private:

};