#include "EnemyBase.h"
#include"../../ObjectManager.h"
#include"../../../Scene/SceneManager.h"
#include"../Player/Player.h"
#include"../Action/Enemy/Enemy_ConText.h"
#include"../../UI/Enemy/Enemy_UI.h"

void EnemyBase::Update()
{
	CharacterBase::Update();

	m_ui->Update();
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

void EnemyBase::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.SetDissolve(m_dossolve);
	CharacterBase::GenerateDepthMapFromLight();
}

void EnemyBase::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.SetDissolve(m_dossolve);
	CharacterBase::DrawLit();
}

void EnemyBase::DrawSprite()
{
	m_ui->DrawSprite();
}

void EnemyBase::Init()
{
	CharacterBase::Init();

	Math::Matrix Trans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = Trans;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Enemy", m_model, KdCollider::TypeDamage | KdCollider::TypeBump);

	m_ObjType = ObjType::oEnemy;

	m_ui = std::make_shared<Enemy_UI>();
	m_ui->SetTarget(shared_from_this());
	m_ui->SetCamera(m_player.lock()->GetCamera().lock());
	m_ui->Init();
}

void EnemyBase::CrushingAction()
{
	if (m_anime != "Death")
	{
		m_anime = "Death";
		m_animeFlg = false;
		m_animeSpeed = 1.0f;
	}

	m_dossolve+=0.01f;
	if (m_dossolve >= 1.0f)
	{
		m_isExpired = true;
	}
}
