#pragma once
#include "../CameraBase.h"

class TPSCamera : public CameraBase
{
public:
	TPSCamera()                         { Init(); }
	~TPSCamera()			override	{}

	void Init()				override;
	void Update()           override;
	void PostUpdate()       override;

private:
	Math::Vector3 m_targetPos = Math::Vector3::Zero;
};