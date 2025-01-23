#pragma once

// 武器基底
#include"../WeaponBase.h"

class Shield :public WeaponBase
{
public:
	Shield() {}
	~Shield()override {};

	void Update()override;

	// ゲッター=============================================================================================================
	// パリィエフェクト位置
	const Math::Matrix& GetParryPoint();
	//======================================================================================================================

private:

};