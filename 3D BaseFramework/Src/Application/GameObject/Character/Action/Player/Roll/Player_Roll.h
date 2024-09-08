#pragma once
#include"../Player_ActionState.h"

class Player_Roll :public Player_ActionState
{
public:
	Player_Roll() {Init();}
	~Player_Roll()  override {};

	void Start()    override;
	void End()      override;

	void Event()    override;

	void Init()     override;

	void Idol(std::shared_ptr<Player_ActionConText> context)          override;             //待ち

private:
};