#include "Bullet.h"
#include"../../../../../Scene/SceneManager.h"
#include"../Golem.h"
#include"../../../Player/Player.h"
#include"../../../Action/Player/Player_ConText.h"

void Bullet::Update()
{
	if (m_size < m_MaxSize)m_size += m_sizeChange;
	else
	{
		m_pos += m_speed * m_dir;
	}

	Math::Matrix _scale = Math::Matrix::CreateScale(m_size);
	Math::Matrix _trans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = _scale * _trans;
}

void Bullet::PostUpdate()
{
	KdCollider::SphereInfo sphereInfo;
	sphereInfo.m_sphere.Center = m_pos;
	sphereInfo.m_sphere.Radius = 2.0f * m_size;
	sphereInfo.m_type = KdCollider::Type::TypeBump | KdCollider::Type::TypeGround | KdCollider::Type::TypeDamage;

	Math::Color _color = { 1,0,0,1 };
	m_pDebugWire->AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius, _color);

	for (auto& ret : SceneManager::Instance().GetObjList())
	{
		if (ret->GetID() == m_id)continue;

		if (ret->Intersects(sphereInfo, nullptr))
		{
			Expired();
			return;
		}
	}

	if (m_golem.lock()->GetTarget().lock()->Intersects(sphereInfo, nullptr))
	{
		m_golem.lock()->GetTarget().lock()->GetConText()->Hit(5, shared_from_this());
		return;
	}
}

void Bullet::Init()
{
	m_model = std::make_shared<KdModelWork>();
	m_model->SetModelData("Asset/Models/Character/Enemy/Golem/Bullet/Bullet.gltf");
	m_size = 0.0f;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Bullet", m_model, KdCollider::TypeDamage | KdCollider::TypeBump);

	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}
