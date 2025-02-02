﻿#include "CameraBase.h"

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

void CameraBase::SetTarget(const std::shared_ptr<Player>& target)
{
	if (!target) { return; }

	m_wpTarget = target;
}

const std::shared_ptr<KdCamera>& CameraBase::GetCamera() const
{
	return m_spCamera;
}

std::shared_ptr<KdCamera> CameraBase::WorkCamera() const
{
	return m_spCamera;
}

const Math::Matrix CameraBase::GetRotationMatrix() const
{
	return Math::Matrix::CreateFromYawPitchRoll(
		DirectX::XMConvertToRadians(m_DegAng.y),
		DirectX::XMConvertToRadians(m_DegAng.x),
		DirectX::XMConvertToRadians(m_DegAng.z));
}

const Math::Matrix CameraBase::GetRotationXMatrix() const
{
	return Math::Matrix::CreateRotationX(
		DirectX::XMConvertToRadians(m_DegAng.x));
}

const Math::Matrix CameraBase::GetRotationYMatrix() const
{
	return Math::Matrix::CreateRotationY(
		DirectX::XMConvertToRadians(m_DegAng.y));
}

const Math::Vector3 CameraBase::GetDegAng() const
{
	return m_DegAng;
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
