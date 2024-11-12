#include "MagicPolygon.h"
#include"../../../Scene/SceneManager.h"
#include"../../StageManager.h"
#include"../Circle/Circle.h"
#include"../../Camera/GameCamera/GameCamera.h"
#include"../../Camera/GameCamera/GameCamera_ConText.h"
#include"MagicPolygon_ConText.h"
#include"MagicPolygon_State.h"
#include"Normal/MagicPolygon_Normal.h"

void MagicPolygon::Update()
{
	if (!SceneManager::Instance().m_stop)
	{
		if (m_NextState)
		{
			m_conText->SetState(m_NextState);
			m_state = m_NextState;
			m_NextState.reset();
		}
		m_state.lock()->Update();

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

	std::shared_ptr<MagicPolygon_Normal> _normal = std::make_shared<MagicPolygon_Normal>();
	m_conText = std::make_shared<MagicPolygon_ConText>(_normal);
	m_state = _normal;
	m_state.lock()->SetTarget(shared_from_this());
	
}
