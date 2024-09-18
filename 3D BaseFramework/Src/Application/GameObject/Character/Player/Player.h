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

	void Action()    override;
	void PostUpdate()override;
	void Init()      override;

	void CrushingAction()override;

	void SetCamera(std::shared_ptr<CameraBase> a_camera) { m_camera = a_camera; }
	void SetNextState(std::shared_ptr<Player_ActionState> _next) { m_NextState = _next; }
	std::weak_ptr<CameraBase> GetCamera()const { return m_camera; }
	std::shared_ptr<Player_ActionConText>GetConText()const { return m_context; }

private:
	std::weak_ptr<CameraBase> m_camera;
	std::shared_ptr<Player_ActionConText> m_context = nullptr;
	std::weak_ptr<Player_ActionState> m_state;
	std::shared_ptr<Player_ActionState> m_NextState;
};