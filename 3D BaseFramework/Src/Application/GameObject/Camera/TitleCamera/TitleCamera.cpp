#include "TitleCamera.h"

void TitleCamera::Update()
{
	// カメラ回転
	m_angle += m_ChangeAngle;
	if (m_angle > 360.0f)m_angle -= 360.0f;

	// 行列
	m_mRotation = GetRotationMatrix(); // 回転(傾き)
	m_mLocalPos = Math::Matrix::CreateTranslation(m_pos); // 座標
	Math::Matrix _rotY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle)); // 回転(公転)
	// 行列合成
	m_mWorld = m_mRotation * m_mLocalPos * _rotY;
}

void TitleCamera::Init()
{
	if (!m_spCamera)
	{
		m_spCamera = std::make_shared<KdCamera>();
	}

	m_spCamera->SetProjectionMatrix(m_ViewingAngle);
}
