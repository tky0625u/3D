﻿#pragma once

class UIBase :public KdGameObject
{
public:
	UIBase()                          {};
	virtual ~UIBase()        override {};

	virtual void Update()    override {};
	virtual void DrawSprite()override;
	virtual void Init()      override {};

protected:
	std::shared_ptr<KdTexture> m_pTex  = nullptr;
	Math::Vector2              m_pos   = Math::Vector2::Zero;
	Math::Vector2              m_pivot = { 0.5f,0.5f };
	Math::Rectangle            m_rect;
	Math::Color                m_color = { 1,1,1,1 };
};