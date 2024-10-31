#include "MagicPolygon.h"
#include"../../../Scene/SceneManager.h"
#include"../../ObjectManager.h"
#include"../Circle/Circle.h"
#include"../../Camera/GameCamera/GameCamera.h"
#include"../../Camera/GameCamera/GameCamera_ConText.h"

void MagicPolygon::Update()
{
	if (!SceneManager::Instance().m_stop)
	{
		if (m_ObjManager.lock()->GetEnemyList().size() == 0)
		{
			if (!m_rightFlg)
			{
				m_ObjManager.lock()->GetCamera().lock()->GetConText()->FixedCamera();
				KdEffekseerManager::GetInstance().Play("Circle/Circle.efkefc", m_mWorld.Translation(), m_size, 1.0f, false);
				m_rightFlg = true;
			}
			m_angle.y += 0.1f;
			if (m_angle.y > 360.0f)m_angle.y -= 360.0f;
			if (m_rgb < 1.0f)m_rgb += 0.01f;
			else
			{
				if (m_ObjManager.lock()->GetCamera().lock()->GetCameraType() == GameCamera::CameraType::FixedType)
				{
					KdEffekseerManager::GetInstance().StopEffect("Circle/Circle.efkefc");
					m_ObjManager.lock()->GetCamera().lock()->GetConText()->PlayerCamera();
				}
			}
		}
		else
		{
			m_rgb = 0.0f;
			m_rightFlg = false;
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
}
