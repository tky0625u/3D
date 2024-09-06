#pragma once
#include"../Enemy_ActionState.h"

class Enemy_Attack :public Enemy_ActionState
{
public:
	Enemy_Attack() { Init(); }
	~Enemy_Attack()   override {};

	void Start()    override;
	void Center()   override;
	void End()      override;

	void Event()override;

	void Idol   (std::shared_ptr<Enemy_ConText> context)override;  //待ち
	void Hit    (std::shared_ptr<Enemy_ConText> context)override;  //被弾
	void Stumble(std::shared_ptr<Enemy_ConText> context)override;  //のけぞり

private:
};