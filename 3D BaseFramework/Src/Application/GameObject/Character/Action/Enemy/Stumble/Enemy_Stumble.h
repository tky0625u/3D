#pragma once
#include"../Enemy_ActionState.h"

class Enemy_Stumble :public Enemy_ActionState
{
public:
	Enemy_Stumble() { Init(); }
	~Enemy_Stumble()override {};

	void Start()override;
	void End()override;

	void Stumble();

	void Idol(std::shared_ptr<Enemy_ConText> context)override;  //待ち
	void Hit (std::shared_ptr<Enemy_ConText> context)override;  //被ダメ

private:

};