#include "Bullet.h"
#include"../../../../../Scene/SceneManager.h"
#include"../../../../ObjectManager.h"
#include"../Golem.h"
#include"../../../Player/Player.h"
#include"../../../Action/Enemy/Enemy_ConText.h"

void Bullet::Update()
{
	if (m_size < m_MaxSize)m_size += m_sizeChange * m_ObjectManager.lock()->GetSlow();
	else
	{
		m_size = m_MaxSize;
		m_pos += m_speed * m_dir * m_ObjectManager.lock()->GetSlow();
	}

	m_angle.x += 5.0f * m_ObjectManager.lock()->GetSlow();
	m_angle.y += 5.0f * m_ObjectManager.lock()->GetSlow();
	m_angle.z += 5.0f * m_ObjectManager.lock()->GetSlow();
	if (m_angle.x > 360.0f)m_angle.x -= 360.0f;
	if (m_angle.y > 360.0f)m_angle.y -= 360.0f;
	if (m_angle.z > 360.0f)m_angle.z -= 360.0f;

	Math::Matrix _scale = Math::Matrix::CreateScale(m_size);
	Math::Matrix _rotX  = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(m_angle.x));
	Math::Matrix _rotY  = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle.y));
	Math::Matrix _rotZ  = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(m_angle.z));
	Math::Matrix _trans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = _scale * _rotX * _rotY * _rotZ * _trans;
}

void Bullet::PostUpdate()
{
	if (m_size < m_MaxSize)return;

	KdCollider::SphereInfo sphereInfo;
	sphereInfo.m_sphere.Center = m_pos;
	sphereInfo.m_sphere.Radius = 1.0f * m_size;
	sphereInfo.m_type = KdCollider::Type::TypeBump | KdCollider::Type::TypeGround | KdCollider::Type::TypeDamage;

	// デバッグ
	//Math::Color _color = { 1,0,0,1 };
	//m_pDebugWire->AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius, _color);

	for (auto& ret : SceneManager::Instance().GetObjList())
	{
		if (ret->GetID() == m_id)continue;

		if (ret->Intersects(sphereInfo, nullptr))
		{
			KdEffekseerManager::GetInstance().Play("Enemy/Golem/Bullet/Bom.efkefc", m_pos, m_size * 0.5f, 1.0f, false);
			Expired();
			return;
		}
	}

	switch (m_owner)
	{
	case KdGameObject::ObjType::oEnemy:
		if (m_golem.lock()->GetTarget().lock()->Intersects(sphereInfo, nullptr))
		{
			m_golem.lock()->GetTarget().lock()->Damage(5, shared_from_this());
			if (m_crush)
			{
				KdEffekseerManager::GetInstance().Play("Enemy/Golem/Bullet/Bom.efkefc", m_pos, m_size * 0.5f, 1.0f, false);
				Expired();
			}
			return;
		}
		break;
	case KdGameObject::ObjType::oPlayer:
		for (auto& ret : SceneManager::Instance().GetEnemyList())
		{
			if (ret->Intersects(sphereInfo, nullptr))
			{
				ret->Damage(5);
				KdEffekseerManager::GetInstance().Play("Enemy/Golem/Bullet/Bom.efkefc", m_pos, m_size * 0.5f, 1.0f, false);
				Expired();
			}
		}
		break;
	default:
		break;
	}

}

void Bullet::Init()
{
	BulletBase::Init();

	m_model = std::make_shared<KdModelWork>();
	m_model->SetModelData("Asset/Models/Character/Enemy/Golem/Bullet/Bullet.gltf");
	m_size = 0.0f;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Bullet", m_model, KdCollider::TypeDamage | KdCollider::TypeBump);

	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}
