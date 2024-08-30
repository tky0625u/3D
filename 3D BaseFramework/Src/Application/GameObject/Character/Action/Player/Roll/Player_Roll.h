#pragma once
#include"../../ActionBase.h"

class Player_Roll :public ActionBase
{
public:
	Player_Roll() {Init();}
	~Player_Roll()  override {};

	void Start()    override;
	void End()      override;

	void Event()    override;

	void Init()     override;

private:

};