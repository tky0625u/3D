#pragma once

#include"../ObjectBase.h"

class Wall :public ObjectBase
{
public:
	Wall()                  {}
	~Wall()        override {};

	void Init()    override;

private:

};