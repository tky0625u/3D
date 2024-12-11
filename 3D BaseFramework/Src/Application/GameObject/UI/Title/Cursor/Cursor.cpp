#include "Cursor.h"
#include"../../../../Scene/SceneManager.h"
#include"Particle/Particle.h"

void Cursor::Update()
{
	if (!SceneManager::Instance().GetBlackAlphaFlg())
	{
		POINT _cursor;
		GetCursorPos(&_cursor);
		m_pos = { float(_cursor.x - 640.0f),float(-_cursor.y + 390.0f) };

		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			if (!m_ClickFlg)
			{
				for (int p = 0; p < m_ParticleNum; ++p)
				{
					std::shared_ptr<Particle> _particle = std::make_shared<Particle>();
					_particle->Init();
					Math::Vector2 _move = { (rand() % 20 - 10 + 1) * 0.1f,(rand() % 20 - 10 + 1) * 0.1f };
					float         _size = (rand() % 10 - 5 + 1) * 0.1f;
					_particle->SetParam(m_pos, _move, _size);
					m_ParticleList.push_back(_particle);
				}
				m_ClickFlg = true;
			}
		}
		else
		{
			m_ClickFlg = false;
		}
	}

	for (auto& _particle : m_ParticleList)_particle->Update();
}

void Cursor::PostUpdate()
{
	ParticleDelete();
}

void Cursor::DrawSprite()
{
	for (auto& _particle : m_ParticleList)_particle->DrawSprite();
	UIBase::DrawSprite();
}

void Cursor::Init()
{
	m_pivot = { 0.0f,1.0f };
	m_rect = { 0,0,long(62),long(97) };
	m_pTex = std::make_shared<KdTexture>();
	m_pTex->Load("Asset/Textures/UI/Title/Cursor/Cursors.png");
	srand(timeGetTime());
}

void Cursor::ParticleDelete()
{
	auto _particle = m_ParticleList.begin();

	while (_particle != m_ParticleList.end())
	{
		if ((*_particle)->IsExpired())
		{
			_particle = m_ParticleList.erase(_particle);
		}
		else
		{
			_particle++;
		}
	}
}
