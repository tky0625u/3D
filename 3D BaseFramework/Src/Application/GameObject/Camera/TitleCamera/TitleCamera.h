#pragma once
#include"../CameraBase.h"

class TitleCamera :public CameraBase
{
public:
	TitleCamera() {};
	~TitleCamera()override {};

	void Update()    override;
	void Init()      override;

	// セッター======================================================
	void SetDegAng(Math::Vector3 _deg) { m_DegAng = _deg; }
	void SetPos(Math::Vector3 _pos)    { m_pos = _pos; }
	void SetViewingAngle(float _angle) { m_ViewingAngle = _angle; }
	void SetChangeAngle(float _angle)  { m_ChangeAngle = _angle; }
	//===============================================================

	// ゲッター======================================================
	const Math::Vector3 GetDegAng()      const { return m_DegAng; }
	const float&        GetViewingAngle()const { return m_ViewingAngle; }
	const float&        GetChangeAngle() const { return m_ChangeAngle; }
	//===============================================================

private:
	float m_ChangeAngle = 0.0f; // 回転角度変化量
	float m_angle       = 0.0f; // 回転角度
};