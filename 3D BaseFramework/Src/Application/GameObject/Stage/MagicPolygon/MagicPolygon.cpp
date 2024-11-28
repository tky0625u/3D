#include "MagicPolygon.h"
#include"../../../Scene/SceneManager.h"
#include"../../StageManager.h"
#include"../Circle/Circle.h"
#include"../../Camera/GameCamera/GameCamera.h"

void MagicPolygon::Update()
{
	if (!SceneManager::Instance().m_stop)
	{
		if (m_NextState)
		{
			m_state = m_NextState;
			m_NextState.reset();
		}
		switch (m_flow)
		{
		case Flow::EnterType:
			m_state->Enter(shared_from_this());
			break;
		case Flow::UpdateType:
			m_state->Update(shared_from_this());
			break;
		case Flow::ExitType:
			m_state->Exit(shared_from_this());
			break;
		}

		m_color = { m_rgb,m_rgb ,m_rgb ,1 };
	}

	Math::Matrix _scale = Math::Matrix::CreateScale(m_size);
	Math::Matrix _rotX= Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(90));
	Math::Matrix _rotY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle.y));
	Math::Matrix _trans = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix _PointTrans;
	if (m_circle.expired() == false)_PointTrans = Math::Matrix::CreateTranslation(m_circle.lock()->GetMagicPolygonPoint());
	m_mWorld = _scale * _rotX * _rotY * _trans * _PointTrans;
}

void MagicPolygon::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_spPolygon, m_mWorld,m_color);
}

void MagicPolygon::DrawBright()
{
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_spPolygon, m_mWorld, m_color);
}

void MagicPolygon::Init()
{
	m_spPolygon = std::make_shared<KdSquarePolygon>();
	m_spPolygon->SetMaterial("Asset/Textures/MagicPolygon/magic circle red.png");

	m_rgb = 0.0f;
	m_color = { m_rgb,m_rgb ,m_rgb ,1 };

	m_angle = Math::Vector3::Zero;

	std::shared_ptr<Normal> _normal = std::make_shared<Normal>();
	m_state = _normal;
}

// Normal==========================================================================================
void MagicPolygon::Normal::Enter(std::shared_ptr<MagicPolygon> owner)
{
	if (owner->m_TeleportFlg)owner->m_TeleportFlg = false;
	owner->m_flow = MagicPolygon::Flow::UpdateType;
}

void MagicPolygon::Normal::Update(std::shared_ptr<MagicPolygon> owner)
{
	if (owner->m_rgb != 0.0f)
	{
		owner->m_rgb = 0.0f;
	}

	if (owner->m_stageManager.lock()->GetnowStage() != owner->m_stageManager.lock()->GetMaxStage() && 
		owner->m_stageManager.lock()->GetnowWave() == owner->m_stageManager.lock()->GetMaxWave()   &&
		SceneManager::Instance().GetEnemyList().size() == 0)ChangeState(owner);
}

void MagicPolygon::Normal::Exit(std::shared_ptr<MagicPolygon> owner)
{
}

void MagicPolygon::Normal::ChangeState(std::shared_ptr<MagicPolygon> owner)
{
	std::shared_ptr<Next> _next = std::make_shared<Next>();
	owner->m_NextState = _next;
	owner->m_flow = MagicPolygon::Flow::EnterType;
	return;
}
//=================================================================================================


// Next============================================================================================
void MagicPolygon::Next::Enter(std::shared_ptr<MagicPolygon> owner)
{
	m_handle = KdEffekseerManager::GetInstance().Play("Circle/Circle.efkefc", owner->m_mWorld.Translation(), owner->m_size, 1.0f, false).lock()->GetHandle();
	owner->m_flow = MagicPolygon::Flow::UpdateType;
}

void MagicPolygon::Next::Update(std::shared_ptr<MagicPolygon> owner)
{
	owner->m_angle.y += 0.01f;
	if (owner->m_angle.y > 360.0f)owner->m_angle.y -= 360.0f;
	if (owner->m_rgb < 1.0f)      owner->m_rgb += 0.01f;
	if (!KdEffekseerManager::GetInstance().IsPlaying(m_handle) && !owner->m_TeleportFlg)owner->m_TeleportFlg = true;

	if (SceneManager::Instance().GetEnemyList().size() != 0 && owner->m_TeleportFlg)ChangeState(owner);
}

void MagicPolygon::Next::Exit(std::shared_ptr<MagicPolygon> owner)
{
}

void MagicPolygon::Next::ChangeState(std::shared_ptr<MagicPolygon> owner)
{
	std::shared_ptr<Normal> _normal = std::make_shared<Normal>();
	owner->m_NextState = _normal;
	owner->m_flow = MagicPolygon::Flow::EnterType;
	return;
}
//=================================================================================================