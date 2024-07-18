#pragma once
#include"../CharacterBase.h"

class CameraBase;

class LeaderBase :public CharacterBase
{
public:
	LeaderBase() {}
	~LeaderBase()override {};

	void Action()override;
	void Init()  override;

protected:
	std::weak_ptr<CameraBase> m_camera;
	const float               m_DashCorrection = 5.0f; //ダッシュ補正
};