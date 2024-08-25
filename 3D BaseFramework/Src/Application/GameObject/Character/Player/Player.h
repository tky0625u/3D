#pragma once

#include"../CharacterBase.h"

class CameraBase;
class ActionBase;

class Player :public CharacterBase
{
public:
	Player()                  {}
	~Player()        override {};

	enum ActionType
	{
		Idol   =1<<0,
		Move   =1<<1,
		Attack =1<<2,
		Guard  =1<<3,
		Roll   =1<<4,
	};

	void PreUpdate() override;
	void Action()    override;
	void Init()      override;

	void CrushingAction()override;

	void SetCamera(std::shared_ptr<CameraBase> a_camera) { m_camera = a_camera; }

private:
	UINT                      m_ActionType = ActionType::Idol;
	UINT                      m_BeforeActionType = m_ActionType;
	std::weak_ptr<CameraBase> m_camera;
};