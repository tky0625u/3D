#pragma once
#include "../CameraBase.h"

class FPSCamera : public CameraBase
{
public:
	FPSCamera()                         { Init(); }
	~FPSCamera()			override	{}

	void Init()				override;
	void Update()			override;
};