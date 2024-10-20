#include "FixedCamera.h"

#include"../../Character/Player/Player.h"

void FixedCamera::Update()
{
	m_mLocalPos = Math::Matrix::CreateTranslation(m_pos);
	m_mRotation = GetRotationMatrix();
	Math::Matrix _targetMat;
	if (m_wpTarget.expired() == false)_targetMat = m_wpTarget.lock()->GetMatrix();
	m_mWorld = m_mRotation * m_mLocalPos * _targetMat;
}

void FixedCamera::Init()
{
	if (!m_spCamera)
	{
		m_spCamera = std::make_shared<KdCamera>();
	}

	//ShowCursor(false);

	// ↓画面中央座標
	m_FixMousePos.x = 640;
	m_FixMousePos.y = 360;
	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);
}
