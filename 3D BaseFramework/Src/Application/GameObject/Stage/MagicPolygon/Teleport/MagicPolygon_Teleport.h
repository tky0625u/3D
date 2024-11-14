#pragma once
#include"../MagicPolygon_State.h"

class MagicPolygon_Teleport :public MagicPolygon_State
{
public:
	MagicPolygon_Teleport()  {};
	~MagicPolygon_Teleport() {};

	void Start() override;
	void Center()override;
	void End()   override;

private:
	
};