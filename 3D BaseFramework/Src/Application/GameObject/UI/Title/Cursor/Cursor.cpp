#include "Cursor.h"
#include"../../../../Scene/SceneManager.h"

void Cursor::Update()
{
	if (SceneManager::Instance().GetBlackAlphaFlg())return;
	//Math::Vector2 _cursorPos = { float(_cursor.x - 640.0f),float(-_cursor.y + 390.0f) };
}

void Cursor::Init()
{

}
