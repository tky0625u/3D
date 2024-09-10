#include "WeaponBase.h"

void WeaponBase::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}

void WeaponBase::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}

void WeaponBase::Init()
{
	m_spModel = std::make_shared<KdModelWork>();
	m_spModel->SetModelData(m_modelPath);
}
