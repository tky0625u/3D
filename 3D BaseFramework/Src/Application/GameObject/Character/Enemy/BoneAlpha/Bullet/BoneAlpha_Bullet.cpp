#include "BoneAlpha_Bullet.h"
#include"../../../../../Scene/SceneManager.h"
#include"../../EnemyBase.h"
#include"../../../Player/Player.h"

void BoneAlpha_Bullet::Update()
{
	if (m_crush)
	{
		Expired();
		return;
	}

	m_dir.y = 0.0f;
	m_dir.Normalize();
	m_pos += m_dir * m_speed;

	Math::Matrix _scale = Math::Matrix::CreateScale(m_size);
	Math::Matrix _trans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = _scale * _trans;

	KdShaderManager::Instance().WorkAmbientController().AddPointLight(Math::Vector3{ 5.0f,5.0f,5.0f }, 5.0f * m_size, m_pos, true);
}

void BoneAlpha_Bullet::PostUpdate()
{
	KdCollider::SphereInfo sphereInfo;
	sphereInfo.m_sphere.Center = m_pos;
	sphereInfo.m_sphere.Radius = 1.5f * m_size;
	sphereInfo.m_type = KdCollider::Type::TypeBump | KdCollider::Type::TypeGround | KdCollider::Type::TypeDamage;

	// デバッグ
	//Math::Color _color = { 1,0,0,1 };
	//m_pDebugWire->AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius, _color);

	for (auto& ret : SceneManager::Instance().GetObjList())
	{
		if (ret->GetID() == m_id)continue;

		if (ret->Intersects(sphereInfo, nullptr))
		{
			KdEffekseerManager::GetInstance().Play("Enemy/BoneAlpha/Bullet/BulletCrush.efkefc", m_pos, m_size * 0.5f, 1.0f, false);
			Expired();
			return;
		}
	}

	switch (m_owner.lock()->GetObjType())
	{
	case KdGameObject::ObjType::oEnemy:
		if (SceneManager::Instance().GetPlayer()->Intersects(sphereInfo, nullptr))
		{
			SceneManager::Instance().GetPlayer()->Damage(5, shared_from_this());
			if (m_crush)
			{
				KdEffekseerManager::GetInstance().Play("Enemy/BoneAlpha/Bullet/BulletCrush.efkefc", m_pos, m_size * 0.5f, 1.0f, false);
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
				KdEffekseerManager::GetInstance().Play("Enemy/BoneAlpha/Bullet/BulletCrush.efkefc", m_pos, m_size * 0.5f, 1.0f, false);
				ret->Damage(5);
				Expired();
			}
		}
		break;
	default:
		break;
	}
}

void BoneAlpha_Bullet::DrawBright()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}

void BoneAlpha_Bullet::Init()
{
	m_model = std::make_shared<KdModelWork>();
	m_model->SetModelData("Asset/Models/Character/Enemy/BoneAlpha/Bullet/Bullet.gltf");
	m_size = 0.5f;
	m_speed = 1.2f;
	m_dir = Math::Vector3::Zero;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Bullet", m_model, KdCollider::Type::TypeDamage);

	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}
