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

	struct Load
	{
		std::shared_ptr<KdTexture> m_tex   = nullptr;
		Math::Matrix               m_Mat   = Math::Matrix::Identity;
		Math::Vector2              m_pos   = Math::Vector2::Zero;
		Math::Vector2              m_pivot = { 0.5f,0.5f };
		Math::Color                m_color;
		Math::Rectangle            m_rect;
		float                      m_alpha = 0.0f;
		float                      m_angle = 0.0f;
		float                      m_size  = 1.0f;
	};

	Load m_load[2];

};