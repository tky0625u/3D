#pragma once
#include"../../Enemy/EnemyBase.h"

class Bone :public EnemyBase
{
public:
	Bone() {}
	~Bone()override {};

	void CrushingAction()override;
	void Init()override;
	
private:

};