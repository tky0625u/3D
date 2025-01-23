#pragma once

// 弾基底
#include"../../../BulletBase/BulletBase.h"

// 弾基底
class BulletBase;
// ゴーレム
class Golem;

class Bullet :public BulletBase
{
public:
	Bullet()                   {};
	~Bullet()         override {};

	void Update()     override;
	void PostUpdate() override;
	void Init()       override;

	// セッター=====================================================================
	void SetGolem(std::shared_ptr<Golem> _golem); // ゴーレム
	//==============================================================================

	// ゲッター=====================================================================
	const float& GetMaxSize()const; // 最大サイズ
	//==============================================================================

private:
	std::weak_ptr<Golem>         m_golem;              // ゴーレム
	float                        m_MaxSize    = 3.0f;  // 最大サイズ
	float                        m_sizeChange = 0.05f; // サイズ変化量
};