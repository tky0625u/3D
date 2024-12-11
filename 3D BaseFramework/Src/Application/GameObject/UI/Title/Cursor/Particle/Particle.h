#pragma once

class Particle :public KdGameObject
{
public:
	Particle()          {};
	~Particle()override {};

	void Update()    override;
	void DrawSprite()override;
	void Init()      override;

	void SetParam(Math::Vector2 _pos, Math::Vector2 _move, float _size)
	{
		m_pos       = _pos;
		m_move      = _move;
		m_size      = _size;
	}

private:
	std::shared_ptr<KdTexture> m_pTex      = nullptr;
	Math::Vector2              m_pos       = Math::Vector2::Zero;
	Math::Vector2              m_move      = Math::Vector2::Zero;
	Math::Rectangle            m_rect;
	Math::Color                m_color     = { 1,1,1,1 };
};