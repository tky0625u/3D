#pragma once

#include"../CharacterBase.h"

class CameraBase;

class Player :public CharacterBase
{
public:
	Player()               { Init(); }
	~Player()     override {};

	void Action() override;
	void Init()   override;

	void CrushingAction()override;

	void SetCamera(std::shared_ptr<CameraBase> a_camera) { m_camera = a_camera; }

private:
	std::weak_ptr<CameraBase> m_camera;
	const float               m_DashCorrection = 5.0f; //ダッシュ補正
};