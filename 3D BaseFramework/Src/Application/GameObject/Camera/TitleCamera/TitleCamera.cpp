#include "TitleCamera.h"

void TitleCamera::Update()
{
	m_angle += 0.05f;
	if (m_angle > 360.0f)m_angle -= 360.0f;

	m_mRotation = GetRotationMatrix();
	m_mLocalPos = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix _rotY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle));
	m_mWorld = m_mRotation * m_mLocalPos * _rotY;
}

void TitleCamera::Init()
{
	if (!m_spCamera)
	{
		m_spCamera = std::make_shared<KdCamera>();
	}

	m_ViewingAngle = 60;
	m_spCamera->SetProjectionMatrix(m_ViewingAngle);
}
