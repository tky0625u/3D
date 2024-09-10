#pragma once
#include"../WeaponBase.h"

class Shield :public WeaponBase
{
public:
	Shield() {}
	~Shield()override {};

	void Update()override;

private:

};