#pragma once

class Player;

class CameraBase : public KdGameObject
{
public:
	CameraBase()						{}
	virtual ~CameraBase()	override	{}

	void Init()				override;
	virtual void PostUpdate()       override;
	void PreDraw()			override;

	void SetTarget(const std::shared_ptr<Player>& target);

	// 「絶対変更しません！見るだけ！」な書き方
	const std::shared_ptr<KdCamera>& GetCamera() const;

	// 「中身弄るかもね」な書き方
	std::shared_ptr<KdCamera> WorkCamera() const;

	virtual const Math::Matrix GetRotationMatrix()const;

	virtual const Math::Matrix GetRotationXMatrix() const;

	virtual const Math::Matrix GetRotationYMatrix() const;

	virtual const Math::Vector3 GetDegAng()const;

private:


protected:
	virtual void UpdateRotateByMouse();

	std::shared_ptr<KdCamera>	m_spCamera		= nullptr;
	std::weak_ptr<Player>	    m_wpTarget;

	Math::Matrix				m_mLocalPos		= Math::Matrix::Identity;
	Math::Matrix				m_mRotation     = Math::Matrix::Identity;

	float                       m_ViewingAngle   = 0.0f;
	float                       m_FocusBackRange = 0.0f;

	// カメラ回転用マウス座標の差分
	POINT						m_FixMousePos{};

	// カメラ回転用角度
	Math::Vector3				m_DegAng = Math::Vector3::Zero;

};