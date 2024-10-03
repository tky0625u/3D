#include "Bone.h"
#include"../EnemyBase.h"
#include"../../Action/Enemy/Enemy_ConText.h"
#include"../../Action/Enemy/Bone/Appeal/Bone_Appeal.h"

void Bone::Init()
{
	EnemyBase::Init();

	m_model->SetModelData("Asset/Models/Character/Enemy/Bone/Bone.gltf");
	m_pos = { 0.0f,0.0f,20.0f };

	std::shared_ptr<Bone_Appeal> appeal = std::make_shared<Bone_Appeal>();
	appeal->SetTarget(shared_from_this());

	m_conText = std::make_shared<Enemy_ConText>(appeal);
	m_state = appeal;

	m_animator->SetAnimation(m_model->GetData()->GetAnimation(m_anime), m_animeFlg);
}