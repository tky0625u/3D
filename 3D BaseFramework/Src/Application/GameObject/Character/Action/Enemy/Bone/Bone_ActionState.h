#pragma once
#include"../Enemy_ActionState.h"

class Bone_ActionState :public Enemy_ActionState
{
public:
	Bone_ActionState()                                                         { Init(); }
	~Bone_ActionState()                                               override {};

	void Idol   ()              override; // 待ち
	void Run    ()              override; // 走り
	void Attack ()              override; // 攻撃
	void Hit    (int _damage)   override; // 被弾
	void Stumble()              override; // のけぞり
	void Crushing()             override; // 撃破

protected:

};