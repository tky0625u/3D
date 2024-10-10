#include "CameraBase.h"

void CameraBase::Init()
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

	m_ViewingAngle = 60;
	m_spCamera->SetProjectionMatrix(m_ViewingAngle);
}

void CameraBase::PostUpdate()
{
	if (!m_spCamera) { return; }

	m_spCamera->SetCameraMatrix(m_mWorld);
	m_spCamera->SetProjectionMatrix(m_ViewingAngle);
}

void CameraBase::PreDraw()
{
	if (!m_spCamera) { return; }

	m_spCamera->SetToShader();
}

void CameraBase::SlowChange(bool _slowFlg)
{
	if (_slowFlg)
	{
		if (m_ViewingAngle > 45)
		{
			m_ViewingAngle -= 2;
		}
		else
		{
			m_ViewingAngle = 45;
		}
	}
	else
	{
		if (m_ViewingAngle < 60)
		{
			m_ViewingAngle += 2;
		}
		else
		{
			m_ViewingAngle = 60;
		}
	}
}

void CameraBase::SetTarget(const std::shared_ptr<KdGameObject>& target)
{
	if (!target) { return; }

	m_wpTarget = target;
}

void CameraBase::UpdateRotateByMouse()
{
	// マウスでカメラを回転させる処理
	POINT _nowPos;
	GetCursorPos(&_nowPos);

	POINT _mouseMove{};
	_mouseMove.x = _nowPos.x - m_FixMousePos.x;
	_mouseMove.y = _nowPos.y - m_FixMousePos.y;

	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);

	// 実際にカメラを回転させる処理(0.15はただの補正値)
	m_DegAng.x += _mouseMove.y * 0.15f;
	m_DegAng.y += _mouseMove.x * 0.15f;

	// 回転制御
	m_DegAng.x = std::clamp(m_DegAng.x, -20.f, 45.f);
}
