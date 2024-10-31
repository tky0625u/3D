#pragma once
#include "../GameCamera_State.h"

class GameCamera;

class ClearCamera : public GameCamera_State
{
public:
	ClearCamera() {}
	~ClearCamera()	override {}

	void Update()    override;
	void PostUpdate()override;

private:

};