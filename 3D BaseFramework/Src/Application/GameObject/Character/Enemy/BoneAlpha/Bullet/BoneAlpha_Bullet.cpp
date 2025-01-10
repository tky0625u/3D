#include "BoneAlpha_Bullet.h"

// シーンマネジャ
#include"../../../../../Scene/SceneManager.h"
// 敵基底
#include"../../EnemyBase.h"
// プレイヤー
#include"../../../Player/Player.h"

void BoneAlpha_Bullet::Update()
{
	// 破壊フラグがONなら
	if (m_crush)
	{
		// 消滅
		Expired();
		return;
	}

	// Y軸は考慮しない
	m_dir.y = 0.0f;
	// 正規化
	m_dir.Normalize();
	// 移動
	m_pos += m_dir * m_speed;

	// 行列
	Math::Matrix _scale = Math::Matrix::CreateScale(m_size);      // 拡縮
	Math::Matrix _trans = Math::Matrix::CreateTranslation(m_pos); // 座標
	m_mWorld = _scale * _trans; // 合成

	// ポイントライト
	KdShaderManager::Instance().WorkAmbientController().AddPointLight(Math::Vector3{ 5.0f,5.0f,5.0f }, 5.0f * m_size, m_pos, true);
}

void BoneAlpha_Bullet::PostUpdate()
{
	// 当たり判定 スフィア判定
	KdCollider::SphereInfo sphereInfo;
	// 中心
	sphereInfo.m_sphere.Center = m_pos;
	// 半径
	sphereInfo.m_sphere.Radius = 1.5f * m_size;
	// 対象
	sphereInfo.m_type = KdCollider::Type::TypeBump | KdCollider::Type::TypeGround | KdCollider::Type::TypeDamage;

	// デバッグ
	//Math::Color _color = { 1,0,0,1 };
	//m_pDebugWire->AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius, _color);

	// 当たり判定
	for (auto& ret : SceneManager::Instance().GetObjList())
	{
		// 自身なら判定をしない
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
	case KdGameObject::ObjType::oEnemy: // 持ち主が敵だった場合
		// プレイヤー当たり判定
		if (SceneManager::Instance().GetPlayer()->Intersects(sphereInfo, nullptr))
		{
			// ダメージ
			SceneManager::Instance().GetPlayer()->Damage(m_atk, shared_from_this());
			if (m_crush)
			{
				KdEffekseerManager::GetInstance().Play("Enemy/BoneAlpha/Bullet/BulletCrush.efkefc", m_pos, m_size * 0.5f, 1.0f, false);
				Expired(); // 消滅
			}
			return;
		}
		break;
	case KdGameObject::ObjType::oPlayer: // 持ち主がプレイヤーだった場合
		for (auto& ret : SceneManager::Instance().GetEnemyList())
		{
			// 敵当たり判定
			if (ret->Intersects(sphereInfo, nullptr))
			{
				KdEffekseerManager::GetInstance().Play("Enemy/BoneAlpha/Bullet/BulletCrush.efkefc", m_pos, m_size * 0.5f, 1.0f, false);
				ret->Damage(m_atk); // ダメージ
				Expired(); // 消滅
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
	m_model = KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Character/Enemy/BoneAlpha/Bullet/Bullet.gltf");
	m_size = 0.5f;
	m_speed = 1.2f;
	m_dir = Math::Vector3::Zero;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Bullet", m_model, KdCollider::Type::TypeDamage);

	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}
