#pragma once
#include"../WeaponBase.h"

class Shield :public WeaponBase
{
public:
	Shield() {}
	~Shield()override {};

	void Update()override;

	Math::Matrix GetParryPoint() { return m_spModel->FindWorkNode("ParryPoint")->m_worldTransform * m_mWorld; }

private:

};