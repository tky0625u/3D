#pragma once
#include"../CameraBase.h"

class TitleCamera :public CameraBase
{
public:
	TitleCamera() {};
	~TitleCamera()override {};

	void Update()    override;
	void Init()      override;

	void SetDegAng(Math::Vector3 _deg) { m_DegAng = _deg; }
	void SetPos(Math::Vector3 _pos)    { m_pos = _pos; }

	const Math::Vector3 GetDegAng()const { return m_DegAng; }

private:

};