#pragma once

#include"../ObjectBase.h"

class Wall :public ObjectBase
{
public:
	Wall() { Init(); }
	~Wall()override {};

	void DrawLit()override;
	void Init()   override;

private:

};