#pragma once
#include"../WeaponBase.h"

class Shield :public WeaponBase
{
public:
	Shield() { Init(); }
	~Shield()override {};

	void Update()override;
	void Init()override;

private:

};