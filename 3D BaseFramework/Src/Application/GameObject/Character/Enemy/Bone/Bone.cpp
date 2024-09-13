#include "Bone.h"
#include"../EnemyBase.h"
#include"../../Action/Enemy/Enemy_ConText.h"
#include"../../Action/Enemy/Bone/Idol/Bone_Idol.h"

void Bone::Init()
{
	EnemyBase::Init();

	m_model->SetModelData("Asset/Models/Character/Enemy/Bone/Bone.gltf");
	m_pos = { 0.0f,0.0f,20.0f };

	std::shared_ptr<Bone_Idol> idol = std::make_shared<Bone_Idol>();
	idol->SetTarget(shared_from_this());

	m_conText = std::make_shared<Enemy_ConText>(idol);
	m_state = idol;

	m_animator->SetAnimation(m_model->GetData()->GetAnimation(m_anime), m_animeFlg);
}