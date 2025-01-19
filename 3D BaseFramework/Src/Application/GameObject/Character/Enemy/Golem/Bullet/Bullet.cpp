#include "Bullet.h"

// シーンマネジャ
#include"../../../../../Scene/SceneManager.h"
// オブジェクトマネジャ
#include"../../../../ObjectManager.h"
// キャラクター基底
#include"../../../CharacterBase.h"
// ゴーレム
#include"../Golem.h"
// プレイヤー
#include"../../../Player/Player.h"

void Bullet::Update()
{
	// ゴーレム（持ち主）が消滅していたら弾も消滅
	if (m_golem.expired())
	{
		Expired();
		return;
	}

	// ゴーレム（持ち主）のHPが０になったらエフェクトを炊いて消滅
	if (m_golem.lock()->GetParam().Hp <= 0)
	{
		KdEffekseerManager::GetInstance().Play("Enemy/Golem/Bullet/Bom.efkefc", m_pos, m_size * 0.5f, 1.0f, false);
		Expired();
		return;
	}

	// 最大サイズになるまで拡大        変化量                   スロー値
	if (m_size < m_MaxSize)m_size += m_sizeChange * m_ObjectManager.lock()->GetSlow();
	// 最大サイズになったら移動
	else
	{
		m_size = m_MaxSize;
		// 移動    速さ     方向              スロー値
		m_pos += m_speed * m_dir * m_ObjectManager.lock()->GetSlow();
	}

	// 弾の回転
	m_angle.x += 5.0f * m_ObjectManager.lock()->GetSlow();
	m_angle.y += 5.0f * m_ObjectManager.lock()->GetSlow();
	m_angle.z += 5.0f * m_ObjectManager.lock()->GetSlow();
	if (m_angle.x > 360.0f)m_angle.x -= 360.0f;
	if (m_angle.y > 360.0f)m_angle.y -= 360.0f;
	if (m_angle.z > 360.0f)m_angle.z -= 360.0f;

	// 行列
	// 拡縮
	Math::Matrix _scale = Math::Matrix::CreateScale(m_size);
	// 回転
	Math::Matrix _rotX  = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(m_angle.x));
	Math::Matrix _rotY  = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle.y));
	Math::Matrix _rotZ  = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(m_angle.z));
	// 座標
	Math::Matrix _trans = Math::Matrix::CreateTranslation(m_pos);
	// 行列合成
	m_mWorld = _scale * _rotX * _rotY * _rotZ * _trans;
}

void Bullet::PostUpdate()
{
	// ゴーレム（持ち主）が消滅orゴーレム（持ち主）のHPが0なら早期リターン
	if (m_golem.expired() || m_golem.lock()->GetParam().Hp <= 0)return;
	// 最大サイズではなかったら早期リターン
	if (m_size < m_MaxSize)return;

	// スフィア判定
	KdCollider::SphereInfo sphereInfo;
	// 中心
	sphereInfo.m_sphere.Center = m_pos;
	// 半径
	sphereInfo.m_sphere.Radius = 1.0f * m_size;
	// 対象
	sphereInfo.m_type = KdCollider::Type::TypeBump | KdCollider::Type::TypeGround | KdCollider::Type::TypeDamage;

	// デバッグ
	//Math::Color _color = { 1,0,0,1 };
	//m_pDebugWire->AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius, _color);

	// オブジェクトの当たり判定
	for (auto& ret : SceneManager::Instance().GetObjList())
	{
		// 自身なら判定をしない
		if (ret->GetID() == m_id)continue;

		// 当たっていたらエフェクトを炊いて消滅
		if (ret->Intersects(sphereInfo, nullptr))
		{
			// エフェクト
			KdEffekseerManager::GetInstance().Play("Enemy/Golem/Bullet/Bom.efkefc", m_pos, m_size * 0.5f, 1.0f, false);
			// SE
			KdAudioManager::Instance().Play("Asset/Sound/Game/SE/Enemy/Golem/bullet/怪獣の足音.wav", 0.1f, false);
			Expired();
			return;
		}
	}

	// 攻撃判定
	switch (m_owner.lock()->GetObjType())
	{
	case KdGameObject::ObjType::oEnemy: // 持ち主が敵なら
		if (m_golem.lock()->GetTarget().lock()->Intersects(sphereInfo, nullptr))
		{
			// ダメージ
			m_golem.lock()->GetTarget().lock()->Damage(m_atk, shared_from_this());
			// エフェクトを炊いて消滅
			if (m_crush)
			{
				// エフェクト
				KdEffekseerManager::GetInstance().Play("Enemy/Golem/Bullet/Bom.efkefc", m_pos, m_size * 0.5f, 1.0f, false);
				// SE
				KdAudioManager::Instance().Play("Asset/Sound/Game/SE/Enemy/Golem/bullet/怪獣の足音.wav", 0.1f, false);
				Expired();
			}
			return;
		}
		break;
	case KdGameObject::ObjType::oPlayer: // 持ち主がプレイヤーなら
		for (auto& ret : SceneManager::Instance().GetEnemyList())
		{
			if (ret->Intersects(sphereInfo, nullptr))
			{
				// ダメージ
				ret->Damage(m_atk);
				// エフェクトを炊いて消滅
				KdEffekseerManager::GetInstance().Play("Enemy/Golem/Bullet/Bom.efkefc", m_pos, m_size * 0.5f, 1.0f, false);
				// SE
				KdAudioManager::Instance().Play("Asset/Sound/Game/SE/Enemy/Golem/bullet/怪獣の足音.wav", 0.1f, false);
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

	m_model = KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Character/Enemy/Golem/Bullet/Bullet.gltf");
	m_size = 0.0f;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Bullet", m_model, KdCollider::TypeDamage | KdCollider::TypeBump);

	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}
