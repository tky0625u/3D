#include "Cursor.h"
#include"../../../../Scene/SceneManager.h"

void Cursor::Update()
{
	if (SceneManager::Instance().GetBlackAlphaFlg())return;
	POINT _cursor;
	GetCursorPos(&_cursor);
	m_pos = { float(_cursor.x - 640.0f),float(-_cursor.y + 390.0f) };
}

void Cursor::Init()
{
	m_pivot = { 0.0f,1.0f };
	m_rect = { 0,0,long(62),long(97) };
	m_pTex = std::make_shared<KdTexture>();
	m_pTex->Load("Asset/Textures/UI/Title/Cursor/Cursors.png");
}
