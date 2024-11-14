#include "EnemyBase.h"
#include"../../ObjectManager.h"
#include"../../../Scene/SceneManager.h"
#include"../Player/Player.h"
#include"../Action/Enemy/Enemy_ConText.h"
#include"../../UI/Enemy/HP/Enemy_HP.h"
#include"../../Camera/GameCamera/GameCamera.h"
#include"../../Camera/GameCamera/GameCamera_ConText.h"
#include"../../Camera/GameCamera/GameCamera_State.h"

void EnemyBase::PostUpdate()
{
	if (m_camera.lock()->GetConText()->GetState()->GetShakeFlg())return;

	CharacterBase::PostUpdate();

	if (m_inviTime > 0)
	{
		m_inviTime--;
	}
}

void EnemyBase::Action()
{
	if (m_NextState != nullptr)
	{
		m_state = m_NextState;
		m_conText->SetState(m_NextState);
		m_NextState.reset();
	}
	if (m_state.expired() == false)m_state.lock()->Update();
}

void EnemyBase::DrawOutLine()
{
	KdShaderManager::Instance().m_StandardShader.SetEnableOutLineDraw(m_LockONFlg);
	if(m_LockONFlg)KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}

void EnemyBase::Init()
{
	CharacterBase::Init();

	Math::Matrix Trans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = Trans;

	m_ObjType = ObjType::oEnemy;

	std::shared_ptr<Enemy_HP> _hp = std::make_shared<Enemy_HP>();
	_hp->SetTarget(shared_from_this());
	_hp->SetCamera(m_camera.lock());
	_hp->Init();
	SceneManager::Instance().AddEnemyUI(_hp);
}

void EnemyBase::CrushingAction()
{
	CharacterBase::CrushingAction();

	if (m_dissolve >= 1.0f)
	{
		m_isExpired = true;
	}
}
