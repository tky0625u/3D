#include "WeaponBase.h"

// キャラクター基底
#include"../Character/CharacterBase.h"

void WeaponBase::Update()
{
	// ディゾルブを持ち主と同じにする
	m_dissolve = m_target.lock()->GetDissolve();
}

void WeaponBase::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.SetDissolve(m_dissolve);

	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}

void WeaponBase::DrawLit()
{
	// ディゾルブ
	Math::Vector3 _color = { 1,0.5f,0 };
	KdShaderManager::Instance().m_StandardShader.SetDissolve(m_dissolve, nullptr, &_color);

	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}

void WeaponBase::Init()
{
	m_spModel = std::make_shared<KdModelWork>();
	m_spModel->SetModelData(m_modelPath);

	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}
