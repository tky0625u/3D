#include "TitleCamera.h"

void TitleCamera::Update()
{
	m_mLocalPos = Math::Matrix::CreateTranslation(m_pos);
	m_mRotation = GetRotationMatrix();
	m_mWorld = m_mLocalPos * m_mRotation;
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
