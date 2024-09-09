#pragma once
#include"../WeaponBase.h"

class Sword :public WeaponBase
{
public:
	Sword() { Init(); }
	~Sword()override {};

	void Update()override;
	void Init()override;

private:

};