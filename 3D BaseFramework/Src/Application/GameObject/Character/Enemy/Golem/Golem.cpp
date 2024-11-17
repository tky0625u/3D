#include "Golem.h"
#include"../EnemyBase.h"
#include"../../Action/Enemy/Enemy_ConText.h"
#include"../../Action/Enemy/Golem/Appeal/Golem_Appeal.h"

void Golem::Init()
{
	EnemyBase::Init();

	m_model->SetModelData("Asset/Models/Character/Enemy/Golem/golem.gltf");

	std::shared_ptr<Golem_Appeal> appeal = std::make_shared<Golem_Appeal>();
	appeal->SetTarget(shared_from_this());

	m_conText = std::make_shared<Enemy_ConText>(appeal);
	m_state = appeal;

	m_animator->SetAnimation(m_model->GetData()->GetAnimation(m_anime), m_animeFlg);

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Enemy", m_model, KdCollider::TypeDamage | KdCollider::TypeBump | KdCollider::TypeSight);
}
