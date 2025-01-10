#pragma once

// UI基底
#include"../../UIBase.h"

// ステージマネジャ
class StageManager;

class Floor :public UIBase
{
public:
	Floor()                    {};
	~Floor()          override {};

	void Update()     override;
	void DrawSprite() override;
	void Init()       override;

	// セッター==========================================================================
	void SetStageManager(std::shared_ptr<StageManager> _stage) { m_StageManager = _stage; } // ステージマネジャ
	void SetPos         (Math::Vector2 _pos);                                               // 座標
	//===================================================================================

private:
	std::weak_ptr<StageManager>m_StageManager;                     // ステージマネジャ

	// 階層数============================================================================
	std::shared_ptr<KdTexture> m_FloorPTex  = nullptr;             // テクスチャ
	Math::Vector2              m_FloorPos   = Math::Vector2::Zero; // 座標
	Math::Vector2              m_FloorPivot = { 0.5f,0.5f };       // 中心位置
	Math::Rectangle            m_FloorRect;                        // 切り取り範囲
	Math::Color                m_FloorColor = { 1,1,1,1 };         // 色
	//===================================================================================
};