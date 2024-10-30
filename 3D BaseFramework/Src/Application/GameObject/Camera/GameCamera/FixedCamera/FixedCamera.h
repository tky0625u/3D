#pragma once
#include "../GameCamera_State.h"

class GameCamera;

class FixedCamera : public GameCamera_State
{
public:
	FixedCamera() {}
	~FixedCamera()	override {}

	void Update()    override;
	void PostUpdate()override;

private:

};