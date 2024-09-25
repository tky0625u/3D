#include "WeaponBase.h"
#include"../Character/CharacterBase.h"

void WeaponBase::Update()
{
	if (m_target.lock()->GetParam().Hp <= 0)
	{
		m_dossolve += 0.01f;
		if (m_dossolve >= 1.0f)m_dossolve = 1.0f;
	}
}

void WeaponBase::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.SetDissolve(m_dossolve);

	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}

void WeaponBase::DrawLit()
{
	Math::Vector3 _color = { 1,0.5f,0 };
	KdShaderManager::Instance().m_StandardShader.SetDissolve(m_dossolve, nullptr, &_color);

	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}

void WeaponBase::Init()
{
	m_spModel = std::make_shared<KdModelWork>();
	m_spModel->SetModelData(m_modelPath);

	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}
