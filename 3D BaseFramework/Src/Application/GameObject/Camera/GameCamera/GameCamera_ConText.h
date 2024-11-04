#pragma once
#include"GameCamera_State.h"

class GameCamera_State;

class GameCamera_ConText
{
public:
	GameCamera_ConText(std::shared_ptr<GameCamera_State> _state) :m_state(_state) {};
	~GameCamera_ConText() {};

	void PlayerCamera() { m_state->PlayerChange(); }
	void FixedCamera()  { m_state->FixedChange(); }
	void ClearCamera()  { m_state->ClearChange(); }

	void SetState(std::shared_ptr<GameCamera_State> _state) { m_state = _state; }

	const std::shared_ptr<GameCamera_State> GetState()const { return m_state; }

private:
	std::shared_ptr<GameCamera_State> m_state;
};