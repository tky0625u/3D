#pragma once
#include "../GameCamera_State.h"

class GameCamera;

class PlayerCamera : public GameCamera_State
{
public:
	PlayerCamera()           {}
	~PlayerCamera()	override {}

	void Update()    override;
	void PostUpdate()override;

	void LockON();

private:
	std::vector<Math::Vector3> m_PlayerPosList;
	int m_MaxPlayerPosListSize = 15;
};