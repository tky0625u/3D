#include "Circle.h"

// 地面
#include"../Ground/Ground.h"

void Circle::Update()
{
	// 行列
	// 拡縮
	Math::Matrix _Scale = Math::Matrix::CreateScale(m_size);
	// 回転
	Math::Matrix _RotY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle.y));
	// 座標
	Math::Matrix _Trans = Math::Matrix::CreateTranslation(m_pos);
	// 地面行列
	Math::Matrix _GroundMat;
	if (!m_target.expired())_GroundMat = m_target.lock()->GetMatrix();

	// 行列合成
	m_mWorld = _Scale * _RotY * _Trans * _GroundMat;
}

void Circle::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}

void Circle::Init()
{
	// モデル生成
	m_spModel = std::make_shared<KdModelWork>();
	// モデル読み込み
	m_spModel->SetModelData("Asset/Models/Stage/Circle/MagicCircle.gltf");

	// 拡縮
	Math::Matrix _Scale = Math::Matrix::CreateScale(m_size);
	// 回転
	Math::Matrix _RotY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle.y));
	// 座標
	Math::Matrix _Trans = Math::Matrix::CreateTranslation(m_pos);

	// 行列合成
	m_mWorld = _Scale * _RotY * _Trans;

	// コライダー生成
	m_pCollider = std::make_unique<KdCollider>();
	// コライダー設定
	m_pCollider->RegisterCollisionShape("Circle", m_spModel, KdCollider::TypeGround | KdCollider::TypeEvent);
}
