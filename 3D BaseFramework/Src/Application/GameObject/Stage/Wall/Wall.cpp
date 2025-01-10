#include"Wall.h"

void Wall::Update()
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

void Wall::DrawLit()
{
	//オブジェクトを裏返す==========================================================
	KdShaderManager::Instance().ChangeRasterizerState(KdRasterizerState::CullNone);

	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);

	KdShaderManager::Instance().ChangeRasterizerState(KdRasterizerState::CullBack);
	//==============================================================================
}

void Wall::Init()
{
	// モデル生成
	m_model = std::make_shared<KdModelWork>();
	// モデル読み込み
	m_model->SetModelData("Asset/Models/Stage/Wall/Wall.gltf");

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
	m_pCollider->RegisterCollisionShape("Stage", m_model, KdCollider::TypeBump);
}