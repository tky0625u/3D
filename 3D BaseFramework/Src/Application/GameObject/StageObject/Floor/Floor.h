#pragma once

#include"../ObjectBase.h"

class Floor :public ObjectBase
{
public:
	Floor()                 {}
	~Floor()       override {};

	void DrawLit() override;
	void Init()    override;

private:

};