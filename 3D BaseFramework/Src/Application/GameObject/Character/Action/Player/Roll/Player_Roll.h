#pragma once
#include"../../ActionBase.h"

class Player_Roll :public ActionBase
{
public:
	Player_Roll() {Init();}
	~Player_Roll()  override {};

	void End()      override;

	void Roll();

	void Init()     override;

private:

};