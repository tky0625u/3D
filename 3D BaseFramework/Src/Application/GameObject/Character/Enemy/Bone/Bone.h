#pragma once
#include"../../Enemy/EnemyBase.h"

class Bone :public EnemyBase,public std::enable_shared_from_this<Bone>
{
public:
	Bone() {}
	~Bone()override {};

	void Init()override;
	
private:

};