#include "Floor.h"

void Floor::Init()
{
	Math::Matrix Trans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = Trans;
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Floor", m_model, KdCollider::TypeGround | KdCollider::TypeBump);
}
