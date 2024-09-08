#pragma once

#include"../CharacterBase.h"

class CameraBase;
class ActionBase;
class Player_ActionConText;
class Player_ActionState;

class Player :public CharacterBase, public std::enable_shared_from_this<Player>
{
public:
	Player()                  {}
	~Player()        override {};

	enum ActionType
	{
		IdolType   =1<<0,
		MoveType   =1<<1,
		AttackType =1<<2,
		GuardType  =1<<3,
		RollType   =1<<4,
	};

	void PreUpdate() override;
	void Action()    override;
	void Init()      override;

	void CrushingAction()override;

	void SetCamera(std::shared_ptr<CameraBase> a_camera) { m_camera = a_camera; }
	std::weak_ptr<CameraBase> GetCamera()const { return m_camera; }
	std::shared_ptr<Player_ActionConText>GetConText()const { return m_context; }

private:
	UINT                      m_ActionType = ActionType::IdolType;
	UINT                      m_BeforeActionType = m_ActionType;
	std::weak_ptr<CameraBase> m_camera;
	std::shared_ptr<Player_ActionConText> m_context = nullptr;
	std::weak_ptr<Player_ActionState> m_state;
};