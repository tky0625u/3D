#pragma once
#include"../../UIBase.h"

class Cursor :public UIBase
{
public:
	Cursor() {};
	~Cursor()override {};

	void Update()    override;
	void Init()      override;

private:

};