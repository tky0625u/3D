#pragma once

#include"../CharacterBase.h"

class CameraBase;

class Player :public CharacterBase
{
public:
	Player()               { Init(); }
	~Player()     override {};

	void Action() override;
	void Init()   override;

	void CrushingAction()override;

	void UpdateRotateByMouse();

	const Math::Matrix GetRotationMatrix()const
	{
		return Math::Matrix::CreateFromYawPitchRoll(
			DirectX::XMConvertToRadians(m_DegAng.y),
			DirectX::XMConvertToRadians(m_DegAng.x),
			DirectX::XMConvertToRadians(m_DegAng.z));
	}

	const Math::Matrix GetRotationYMatrix() const
	{
		return Math::Matrix::CreateRotationY(
			DirectX::XMConvertToRadians(m_DegAng.y));
	}

	void SetCamera(std::shared_ptr<CameraBase> a_camera) { m_camera = a_camera; }

private:
	std::weak_ptr<CameraBase> m_camera;
	POINT					  m_FixMousePos{};
	Math::Vector3			  m_DegAng = Math::Vector3::Zero;
};