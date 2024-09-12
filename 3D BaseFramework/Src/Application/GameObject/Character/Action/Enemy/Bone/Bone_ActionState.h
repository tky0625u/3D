#pragma once
#include"../Enemy_ActionState.h"

class Bone_ActionState :public Enemy_ActionState
{
public:
	Bone_ActionState()                                                         { Init(); }
	~Bone_ActionState()                                               override {};

	void Idol   (std::shared_ptr<Enemy_ConText> context)              override; //待ち
	void Run    (std::shared_ptr<Enemy_ConText> context)              override; //走り
	void Attack (std::shared_ptr<Enemy_ConText> context)              override; //攻撃
	void Hit    (std::shared_ptr<Enemy_ConText> context, int _damage) override; //被弾
	void Stumble(std::shared_ptr<Enemy_ConText> context)              override; //のけぞり

protected:

};