#pragma once
#include"MagicPolygon_State.h"

class MagicPolygon_State;

class MagicPolygon_ConText
{
public:
	MagicPolygon_ConText(std::shared_ptr<MagicPolygon_State> _state) :m_state(_state) {};
	~MagicPolygon_ConText() {};

	void Normal() { m_state->Normal(); }
	void Next()   { m_state->Next(); }

	void SetState(std::shared_ptr<MagicPolygon_State> _state) { m_state = _state; }

	const std::shared_ptr<MagicPolygon_State>& GetState() { return m_state; }

private:
	std::shared_ptr<MagicPolygon_State> m_state = nullptr;
};