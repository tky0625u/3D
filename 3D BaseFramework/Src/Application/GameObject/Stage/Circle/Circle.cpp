#include "Circle.h"

void Circle::Update()
{
	Math::Matrix _Scale = Math::Matrix::CreateScale(m_size);
	Math::Matrix _RotY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle));
	Math::Matrix _Trans = Math::Matrix::CreateTranslation(m_pos);

	m_mWorld = _Scale * _RotY * _Trans;
}

void Circle::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}

void Circle::Init()
{
	m_spModel = std::make_shared<KdModelWork>();
	m_spModel->SetModelData("Asset/Models/Stage/Circle/MagicCircle.gltf");

	Math::Matrix _Scale = Math::Matrix::CreateScale(m_size);
	Math::Matrix _RotY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle));
	Math::Matrix _Trans = Math::Matrix::CreateTranslation(m_pos);

	m_mWorld = _Scale * _RotY * _Trans;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Circle", m_spModel, KdCollider::TypeGround | KdCollider::TypeEvent);
}
