#pragma once

class Particle :public KdGameObject
{
public:
	Particle()          {};
	~Particle()override {};

	void Update()    override;
	void DrawSprite()override;
	void Init()      override;

	// セッター================================================================
	void SetParam(Math::Vector2 _pos, Math::Vector2 _move, float _size, float _sizeChange) // パラメーター
	{
		m_pos  = _pos;
		m_move = _move;
		m_size = _size;
		m_SizeChange = _sizeChange;
	}
	//=========================================================================

private:
	std::shared_ptr<KdTexture> m_pTex       = nullptr;             // テクスチャ
	Math::Vector2              m_pos        = Math::Vector2::Zero; // 座標
	Math::Vector2              m_move       = Math::Vector2::Zero; // 移動量
	Math::Rectangle            m_rect;                             // 切り取り範囲
	Math::Color                m_color      = { 1,1,1,1 };         // 色
	float                      m_SizeChange = 0.0f;                // サイズ変化量
};