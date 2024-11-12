#pragma once
#include"../MagicPolygon_State.h"

class MagicPolygon_Normal :public MagicPolygon_State
{
public:
	MagicPolygon_Normal()  {};
	~MagicPolygon_Normal() {};

	void Start() override;
	void Center()override;

private:

};