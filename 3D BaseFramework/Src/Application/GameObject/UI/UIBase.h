#pragma once

class UIBase :public KdGameObject
{
public:
	UIBase()                          {};
	virtual ~UIBase()        override {};

	virtual void Update()    override {};
	virtual void DrawSprite()override;
	virtual void Init()      override {};

	// セッター================================================================
	virtual void SetPos(Math::Vector2 _pos); // 座標
	//=========================================================================

	// ゲッター================================================================
	const Math::Vector2& GetVector2Pos()const; // 座標
	//=========================================================================

protected:
	// テクスチャ
	std::shared_ptr<KdTexture> m_pTex  = nullptr;
	// 座標
	Math::Vector2              m_pos   = Math::Vector2::Zero;
	// 中心位置
	Math::Vector2              m_pivot = { 0.5f,0.5f };
	// 切り取り範囲
	Math::Rectangle            m_rect;
	// 色
	Math::Color                m_color = { 1,1,1,1 };
};