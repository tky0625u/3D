#include "EnemyBase.h"
#include"../../ObjectManager.h"
#include"../../../Scene/SceneManager.h"
#include"../Player/Player.h"
#include"../Action/Enemy/Enemy_ConText.h"
#include"../../UI/Enemy/HP/Enemy_HP.h"

void EnemyBase::Update()
{
	CharacterBase::Update();

	m_hp->Update();
}

void EnemyBase::PostUpdate()
{
	CharacterBase::PostUpdate();

	if (m_inviTime > 0 && ObjectManager::Instance().GetStopTime() == 0)
	{
		m_inviTime--;
	}
}

void EnemyBase::Action()
{
	if (m_NextState != nullptr)
	{
		m_state = m_NextState;
		m_NextState.reset();
	}
	if (m_state.expired() == false)m_state.lock()->Update();
}

void EnemyBase::DrawSprite()
{
	m_hp->DrawSprite();
}

void EnemyBase::Init()
{
	CharacterBase::Init();

	Math::Matrix Trans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = Trans;

	m_ObjType = ObjType::oEnemy;

	m_hp = std::make_shared<Enemy_HP>();
	m_hp->SetTarget(shared_from_this());
	m_hp->SetCamera(m_player.lock()->GetCamera().lock());
	m_hp->Init();
}

void EnemyBase::CrushingAction()
{
	CharacterBase::CrushingAction();

	if (m_dossolve >= 1.0f)
	{
		m_isExpired = true;
	}
}
