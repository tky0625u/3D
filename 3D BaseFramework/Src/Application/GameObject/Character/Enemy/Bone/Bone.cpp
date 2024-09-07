#include "Bone.h"
#include"../EnemyBase.h"
#include"../../Action/Enemy/Enemy_ConText.h"
#include"../../Action/Enemy/Idol/Enemy_Idol.h"

void Bone::Init()
{
	EnemyBase::Init();

	m_model->SetModelData("Asset/Models/Character/Enemy/Bone/Bone.gltf");
	m_pos = { 0.0f,0.0f,20.0f };

	std::shared_ptr<Enemy_Idol> idol = std::make_shared<Enemy_Idol>();
	idol->SetTarget(shared_from_this());

	m_conText = std::make_shared<Enemy_ConText>(idol);
	m_conText->SetPlayer(m_player.lock());
	idol->SetConText(m_conText);
	m_state = m_conText->GetState();
	if (m_player.expired() == false)
	{
		m_conText->SetPlayer(m_player.lock());
		idol->SetPlayer(m_player.lock());
	}

	m_animator->SetAnimation(m_model->GetData()->GetAnimation(m_anime), m_animeFlg);
}