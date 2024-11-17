#include "EnemyBase.h"
#include"../../ObjectManager.h"
#include"../../../Scene/SceneManager.h"
#include"../Player/Player.h"
#include"../Action/Enemy/Enemy_ConText.h"
#include"../../Camera/GameCamera/GameCamera.h"
#include"../../Camera/GameCamera/GameCamera_ConText.h"
#include"../../Camera/GameCamera/GameCamera_State.h"

void EnemyBase::PostUpdate()
{
	if (m_camera.lock()->GetConText()->GetState()->GetShakeFlg())return;

	CharacterBase::PostUpdate();

	KdCollider::SphereInfo sphereInfo;
	Math::Matrix _mat = m_model->FindWorkNode("spine")->m_worldTransform * (Math::Matrix::CreateTranslation(m_mWorld.Translation()));
	sphereInfo.m_sphere.Center = _mat.Translation();
	sphereInfo.m_sphere.Radius = 1.0f;
	sphereInfo.m_type = KdCollider::TypeBump;

	//デバッグ用
	m_pDebugWire->AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius, Math::Color{ 0,1,1,1 });

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
}

void EnemyBase::CrushingAction()
{
	CharacterBase::CrushingAction();

	if (m_dissolve >= 1.0f)
	{
		m_isExpired = true;
	}
}
