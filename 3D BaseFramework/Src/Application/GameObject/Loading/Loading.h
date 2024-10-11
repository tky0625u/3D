#pragma once

class Loading :public KdGameObject
{
public:
	Loading() {};
	~Loading()override {};

	void Update()    override;
	void DrawSprite()override;
	void Init()      override;

private:
	std::shared_ptr<KdTexture> m_tex = nullptr;
	Math::Vector2              m_pos = Math::Vector2::Zero;
	Math::Color                m_color;
	Math::Rectangle            m_rect;
	float                      m_alpha = 0.0f;
	float                      m_angle = 0.0f;
	float                      m_size  = 1.0f;


};