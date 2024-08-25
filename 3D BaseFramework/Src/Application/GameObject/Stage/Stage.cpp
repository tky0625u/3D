#include "Stage.h"

void Stage::DrawLit()
{
	//オブジェクトを裏返す==========================================================
	KdShaderManager::Instance().ChangeRasterizerState(KdRasterizerState::CullNone);

	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);

	KdShaderManager::Instance().ChangeRasterizerState(KdRasterizerState::CullBack);
	//==============================================================================
}

void Stage::Init()
{
	m_model = std::make_shared<KdModelWork>();
	m_model->SetModelData("Asset/Models/Stage/Stage.gltf");

	m_pos = { 0.0f,0.0f,-1.0f };

	Math::Matrix Scale = Math::Matrix::CreateScale(10.0f);
	Math::Matrix Trans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = Scale * Trans;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Stage", m_model, KdCollider::TypeGround);
}
