#pragma once

class Player;

class CameraBase : public KdGameObject
{
public:
	CameraBase()						{}
	virtual ~CameraBase()	override	{}

	void Init()				override;
	void PostUpdate()       override;
	void PreDraw()			override;

	void SlowChange(bool _slowFlg);

	void SetTarget(const std::shared_ptr<Player>& target);

	// 「絶対変更しません！見るだけ！」な書き方
	const std::shared_ptr<KdCamera>& GetCamera() const
	{
		return m_spCamera;
	}

	// 「中身弄るかもね」な書き方
	std::shared_ptr<KdCamera> WorkCamera() const
	{
		return m_spCamera;
	}

	const Math::Matrix GetRotationMatrix()const
	{
		return Math::Matrix::CreateFromYawPitchRoll(
			DirectX::XMConvertToRadians(m_DegAng.y),
			DirectX::XMConvertToRadians(m_DegAng.x),
			DirectX::XMConvertToRadians(m_DegAng.z));
	}

	const Math::Matrix GetRotationXMatrix() const
	{
		return Math::Matrix::CreateRotationX(
			DirectX::XMConvertToRadians(m_DegAng.x));
	}

	const Math::Matrix GetRotationYMatrix() const
	{
		return Math::Matrix::CreateRotationY(
			DirectX::XMConvertToRadians(m_DegAng.y));
	}

	const Math::Vector3 GetDegAng()const { return m_DegAng; }

private:


protected:
	void UpdateRotateByMouse();

	std::shared_ptr<KdCamera>	m_spCamera		= nullptr;
	std::weak_ptr<Player>	m_wpTarget;

	Math::Matrix				m_mLocalPos		= Math::Matrix::Identity;
	Math::Matrix				m_mRotation     = Math::Matrix::Identity;

	float                       m_ViewingAngle;

	// カメラ回転用マウス座標の差分
	POINT						m_FixMousePos{};

	// カメラ回転用角度
	Math::Vector3				m_DegAng = Math::Vector3::Zero;

};