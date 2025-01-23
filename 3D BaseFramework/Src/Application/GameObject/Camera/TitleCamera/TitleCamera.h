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
	void SetDegAng(Math::Vector3 _deg);
	void SetViewingAngle(float _angle);
	void SetChangeAngle(float _angle);
	//===============================================================

	// ゲッター======================================================
	const Math::Vector3 GetDegAng()const;
	const float&        GetViewingAngle()const;
	const float&        GetChangeAngle()const;
	//===============================================================

private:
	float m_ChangeAngle = 0.0f; // 回転角度変化量
	float m_angle       = 0.0f; // 回転角度
};