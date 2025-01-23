#pragma once

// 弾基底
#include"../../../BulletBase/BulletBase.h"

class BoneAlpha_Bullet :public BulletBase
{
public:
	BoneAlpha_Bullet() {};
	~BoneAlpha_Bullet()override {};

	void Update()    override;
	void PostUpdate()override;
	void DrawBright()override;
	void Init()      override;

private:

};