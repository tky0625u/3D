#include "EnemyManager.h"
#include"EnemyBase.h"
void EnemyManager::DeleteEnemyList()
{
	auto enemy = m_EnemyList.begin();

	while (enemy != m_EnemyList.end())
	{
		if ((*enemy)->IsExpired())
		{
			enemy = m_EnemyList.erase(enemy);
		}
		else
		{
			++enemy;
		}
	}
}

void EnemyManager::PreUpdate()
{
	for (auto& enemy : m_EnemyList) enemy->PreUpdate();
}

void EnemyManager::Update()
{
	for (auto& enemy : m_EnemyList) enemy->Update();
}

void EnemyManager::PostUpdate()
{
	for (auto& enemy : m_EnemyList) enemy->PostUpdate();
}

void EnemyManager::GenerateDepthMapFromLight()
{
	for (auto& enemy : m_EnemyList) enemy->GenerateDepthMapFromLight();
}

void EnemyManager::PreDraw()
{
	for (auto& enemy : m_EnemyList) enemy->PreDraw();
}

void EnemyManager::DrawLit()
{
	for (auto& enemy : m_EnemyList) enemy->DrawLit();
}

void EnemyManager::DrawUnLit()
{
	for (auto& enemy : m_EnemyList) enemy->DrawUnLit();
}

void EnemyManager::DrawBright()
{
	for (auto& enemy : m_EnemyList) enemy->DrawBright();
}

void EnemyManager::DrawSprite()
{
	for (auto& enemy : m_EnemyList) enemy->DrawSprite();
}

void EnemyManager::Init()
{
	for (auto& enemy : m_EnemyList) enemy->Init();
}
