#include "Bone.h"
#include"../EnemyBase.h"
#include"../../Action/Enemy/Enemy_ConText.h"
#include"../../Action/Enemy/Bone/Appeal/Bone_Appeal.h"

void Bone::CrushingAction()
{
	if (m_anime != "Death")
	{
		m_anime = "Death";
		m_animeFlg = false;
		m_animeSpeed = 1.0f;
		//KdAudioManager::Instance().Play("Asset/Sound/Game/SE/Enemy/Bone/骨を折る.WAV", 0.1f, false);
	}

	m_dossolve += 0.01f;
	
	if (m_dossolve >= 1.0f)
	{
		m_isExpired = true;
	}
}

void Bone::Init()
{
	EnemyBase::Init();

	m_model->SetModelData("Asset/Models/Character/Enemy/Bone/Bone.gltf");

	std::shared_ptr<Bone_Appeal> appeal = std::make_shared<Bone_Appeal>();
	appeal->SetTarget(shared_from_this());

	m_conText = std::make_shared<Enemy_ConText>(appeal);
	m_state = appeal;
	m_state.lock()->SetObjectManager(m_ObjManager.lock());

	m_animator->SetAnimation(m_model->GetData()->GetAnimation(m_anime), m_animeFlg);

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Enemy", m_model, KdCollider::TypeDamage | KdCollider::TypeBump | KdCollider::TypeSight);
}