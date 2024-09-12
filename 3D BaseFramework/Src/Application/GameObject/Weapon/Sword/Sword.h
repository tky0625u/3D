#pragma once
#include"../WeaponBase.h"

class Sword :public WeaponBase
{
public:
	Sword() {}
	~Sword()override {};

	void Update()override;
	void PostUpdate()override;

	Math::Matrix GetModelTop() { return m_spModel->FindWorkNode("sword.Top")->m_worldTransform * m_mWorld; }
	Math::Matrix GetModelCenter() { return m_spModel->FindWorkNode("sword.Center")->m_localTransform * m_spModel->FindWorkNode("sword.Top")->m_worldTransform * m_mWorld; }
	Math::Matrix GetModelBottom() { return m_spModel->FindWorkNode("sword.Bottom")->m_localTransform * m_spModel->FindWorkNode("sword.Center")->m_worldTransform * m_mWorld; }

private:

};