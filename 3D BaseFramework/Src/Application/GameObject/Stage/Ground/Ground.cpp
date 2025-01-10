#include "Ground.h"

void Ground::Update()
{
	// 行列
	// 拡縮
	Math::Matrix Scale = Math::Matrix::CreateScale(m_size);
	// 回転
	Math::Matrix RotY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle.y));
	// 座標
	Math::Matrix Trans = Math::Matrix::CreateTranslation(m_pos);
	// 行列合成
	m_mWorld = Scale * RotY * Trans;
}

void Ground::DrawLit()
{
	//オブジェクトを裏返す==========================================================
	KdShaderManager::Instance().ChangeRasterizerState(KdRasterizerState::CullNone);

	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);

	KdShaderManager::Instance().ChangeRasterizerState(KdRasterizerState::CullBack);
	//==============================================================================
}

void Ground::Init()
{
	// モデル生成
	m_model = std::make_shared<KdModelWork>();
	// モデル読み込み
	m_model->SetModelData("Asset/Models/Stage/Ground/Ground.gltf");

	// 行列
	// 拡縮
	Math::Matrix Scale = Math::Matrix::CreateScale(m_size);
	// 回転
	Math::Matrix RotY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle.y));
	// 座標
	Math::Matrix Trans = Math::Matrix::CreateTranslation(m_pos);
	// 行列合成
	m_mWorld = Scale * RotY * Trans;

	// コライダー生成
	m_pCollider = std::make_unique<KdCollider>();
	// コライダー設定
	m_pCollider->RegisterCollisionShape("Ground", m_model, KdCollider::TypeGround);
}
