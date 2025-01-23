#include "Particle.h"

void Particle::Update()
{
	if (m_size <= 0) // サイズが0以下になったら消滅
	{
		Expired();
		return;
	}

	m_pos += m_move; // 移動
	m_size -= m_SizeChange; // サイズ減少
}

void Particle::DrawSprite()
{
	KdShaderManager::Instance().ChangeBlendState(KdBlendState::Add);

	KdShaderManager::Instance().m_spriteShader.DrawTex(m_pTex, int(m_pos.x), int(m_pos.y), int(m_rect.width * m_size), int(m_rect.height * m_size), &m_rect, &m_color);

	KdShaderManager::Instance().ChangeBlendState(KdBlendState::Alpha);
}

void Particle::Init()
{
	m_pTex = std::make_shared<KdTexture>();
	m_pTex->Load("Asset/Textures/UI/Title/Cursor/Particle/Particle02.png");
	m_size = 0.5f;
	m_rect = { 0,0,128,128 };
}

// セッター========================================================================================
// パラメータ
void Particle::SetParam(Math::Vector2 _pos, Math::Vector2 _move, float _size, float _sizeChange)
{
	m_pos = _pos;
	m_move = _move;
	m_size = _size;
	m_SizeChange = _sizeChange;
}
//=================================================================================================