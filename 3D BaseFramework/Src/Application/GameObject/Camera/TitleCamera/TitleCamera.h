#pragma once
#include"../CameraBase.h"

class TitleCamera :public CameraBase
{
public:
	TitleCamera() {};
	~TitleCamera()override {};

	void Update()    override;
	void Init()      override;

private:

};