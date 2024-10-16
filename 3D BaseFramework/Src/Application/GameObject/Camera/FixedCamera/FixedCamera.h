#pragma once
#include"../CameraBase.h"

class FixedCamera :public CameraBase
{
public:
	FixedCamera() {};
	~FixedCamera()override {};

	void Update()override;
	void Init()  override;

private:

};