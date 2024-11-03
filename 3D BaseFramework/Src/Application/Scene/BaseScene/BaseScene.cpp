#include "BaseScene.h"
#include"../SceneManager.h"
#include"../../GameObject/Camera/CameraBase.h"
#include"../../GameObject/Character/Player/Player.h"
#include"../../GameObject/Character/Enemy/EnemyBase.h"
#include"../../GameObject/Weapon/WeaponBase.h"
#include"../../GameObject/UI/UIBase.h"

void BaseScene::PreUpdate()
{
	DeleteObjectList();
	DeleteEnemyList();
	DeleteWeaponList();
	DeleteUIList();

	if (SceneManager::Instance().m_stop)return;
	
	if (m_player)m_player->PreUpdate();
	for (auto& obj    : m_ObjectList)obj   ->PreUpdate();
	for (auto& enemy  : m_EnemyList) enemy ->PreUpdate();
	for (auto& weapon : m_WeaponList)weapon->PreUpdate();
	for (auto& ui     : m_UIList)    ui    ->PreUpdate();
	if (m_camera)m_camera->PreUpdate();
}

void BaseScene::Update()
{
	if (m_player)m_player->Update();
	for (auto& obj    : m_ObjectList)obj   ->Update();
	for (auto& enemy  : m_EnemyList) enemy ->Update();
	for (auto& weapon : m_WeaponList)weapon->Update();
	for (auto& ui     : m_UIList)    ui    ->Update();
	if (m_camera)m_camera->Update();

	// シーン毎のイベント処理
	Event();
}

void BaseScene::PostUpdate()
{
	if (m_player)m_player->PostUpdate();
	for (auto& obj    : m_ObjectList)obj   ->PostUpdate();
	for (auto& enemy  : m_EnemyList) enemy ->PostUpdate();
	for (auto& weapon : m_WeaponList)weapon->PostUpdate();
	for (auto& ui     : m_UIList)    ui    ->PostUpdate();
	if (m_camera)m_camera->PostUpdate();
}

void BaseScene::PreDraw()
{
	if (m_player)m_player->PreDraw();
	for (auto& obj    : m_ObjectList)obj   ->PreDraw();
	for (auto& enemy  : m_EnemyList) enemy ->PreDraw();
	for (auto& weapon : m_WeaponList)weapon->PreDraw();
	for (auto& ui     : m_UIList)    ui    ->PreDraw();
	if (m_camera)m_camera->PreDraw();
}

void BaseScene::Draw()
{
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 光を遮るオブジェクト(不透明な物体や2Dキャラ)はBeginとEndの間にまとめてDrawする
	KdShaderManager::Instance().m_StandardShader.BeginGenerateDepthMapFromLight();
	{
		if (m_player)m_player->GenerateDepthMapFromLight();
		for (auto& obj    : m_ObjectList)obj   ->GenerateDepthMapFromLight();
		for (auto& enemy  : m_EnemyList) enemy ->GenerateDepthMapFromLight();
		for (auto& weapon : m_WeaponList)weapon->GenerateDepthMapFromLight();
		for (auto& ui     : m_UIList)    ui    ->GenerateDepthMapFromLight();
		if (m_camera)m_camera->GenerateDepthMapFromLight();
	}
	KdShaderManager::Instance().m_StandardShader.EndGenerateDepthMapFromLight();

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 陰影のあるオブジェクト(不透明な物体や2Dキャラ)はBeginとEndの間にまとめてDrawする
	KdShaderManager::Instance().m_StandardShader.BeginLit();
	{
		if (m_player)m_player->DrawLit();
		for (auto& obj    : m_ObjectList)obj   ->DrawLit();
		for (auto& enemy  : m_EnemyList) enemy ->DrawLit();
		for (auto& weapon : m_WeaponList)weapon->DrawLit();
		for (auto& ui     : m_UIList)    ui    ->DrawLit();
		if (m_camera)m_camera->DrawLit();
	}
	KdShaderManager::Instance().m_StandardShader.EndLit();

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 陰影のないオブジェクト(透明な部分を含む物体やエフェクト)はBeginとEndの間にまとめてDrawする
	KdShaderManager::Instance().m_StandardShader.BeginUnLit();
	{
		if (m_player)m_player->DrawUnLit();
		for (auto& obj    : m_ObjectList)obj   ->DrawUnLit();
		for (auto& enemy  : m_EnemyList) enemy ->DrawUnLit();
		for (auto& weapon : m_WeaponList)weapon->DrawUnLit();
		for (auto& ui     : m_UIList)    ui    ->DrawUnLit();
		if (m_camera)m_camera->DrawUnLit();
	}
	KdShaderManager::Instance().m_StandardShader.EndUnLit();

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 光源オブジェクト(自ら光るオブジェクトやエフェクト)はBeginとEndの間にまとめてDrawする
	KdShaderManager::Instance().m_postProcessShader.BeginBright();
	{
		if (m_player)m_player->DrawBright();
		for (auto& obj    : m_ObjectList)obj   ->DrawBright();
		for (auto& enemy  : m_EnemyList) enemy ->DrawBright();
		for (auto& weapon : m_WeaponList)weapon->DrawBright();
		for (auto& ui     : m_UIList)    ui    ->DrawBright();
		if (m_camera)m_camera->DrawBright();
	}
	KdShaderManager::Instance().m_postProcessShader.EndBright();

	KdEffekseerManager::GetInstance().Draw();
}

void BaseScene::DrawSprite()
{
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 2Dの描画はこの間で行う
	KdShaderManager::Instance().m_spriteShader.Begin();
	{
		if (m_player)m_player->DrawSprite();
		for (auto& obj    : m_ObjectList)obj   ->DrawSprite();
		for (auto& enemy  : m_EnemyList) enemy ->DrawSprite();
		for (auto& weapon : m_WeaponList)weapon->DrawSprite();
		for (auto& ui     : m_UIList)    ui    ->DrawSprite();
		if (m_camera)m_camera->DrawSprite();
	}
	KdShaderManager::Instance().m_spriteShader.End();
}

void BaseScene::DrawDebug()
{
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// デバッグ情報の描画はこの間で行う
	KdShaderManager::Instance().m_StandardShader.BeginUnLit();
	{
		if (m_player)m_player->DrawDebug();
		for (auto& obj    : m_ObjectList)obj   ->DrawDebug();
		for (auto& enemy  : m_EnemyList) enemy ->DrawDebug();
		for (auto& weapon : m_WeaponList)weapon->DrawDebug();
		for (auto& ui     : m_UIList)    ui    ->DrawDebug();
		if (m_camera)m_camera->DrawDebug();
	}
	KdShaderManager::Instance().m_StandardShader.EndUnLit();
}

void BaseScene::DeleteEnemyList()
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

void BaseScene::DeleteWeaponList()
{
	auto weapon = m_WeaponList.begin();

	while (weapon != m_WeaponList.end())
	{
		if ((*weapon)->IsExpired())
		{
			weapon = m_WeaponList.erase(weapon);
		}
		else
		{
			++weapon;
		}
	}

}

void BaseScene::DeleteObjectList()
{
	auto obj = m_ObjectList.begin();

	while (obj != m_ObjectList.end())
	{
		if ((*obj)->IsExpired())
		{
			obj = m_ObjectList.erase(obj);
		}
		else
		{
			++obj;
		}
	}

}

void BaseScene::DeleteUIList()
{
	auto ui = m_UIList.begin();

	while (ui != m_UIList.end())
	{
		if ((*ui)->IsExpired())
		{
			ui = m_UIList.erase(ui);
		}
		else
		{
			++ui;
		}
	}

}

void BaseScene::Event()
{
	// 各シーンで必要な内容を実装(オーバーライド)する
}

void BaseScene::Init()
{
	// 各シーンで必要な内容を実装(オーバーライド)する
}
