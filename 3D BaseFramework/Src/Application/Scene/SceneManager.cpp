#include "SceneManager.h"

#include"../GameObject/ObjectManager.h"
#include "BaseScene/BaseScene.h"
#include "TitleScene/TitleScene.h"
#include "GameScene/GameScene.h"

void SceneManager::PreUpdate()
{
	// シーン切替
	if (m_currentSceneType != m_nextSceneType)
	{
		ChangeScene(m_nextSceneType);
	}

	m_currentScene->PreUpdate();
}

void SceneManager::Update()
{
	m_currentScene->Update();

	if (m_blackChangeFlg)
	{
		if (m_blackAlpha < 1.0f)m_blackAlpha += m_blackChange;
		else { m_blackAlpha = 1.0f; }
	}
	else
	{
		if (m_blackAlpha > 0.0f)m_blackAlpha -= m_blackChange;
		else { m_blackAlpha = 0.0f; }
	}
	m_black = { 0.0f,0.0f,0.0f,m_blackAlpha };
}

void SceneManager::PostUpdate()
{
	m_currentScene->PostUpdate();
}

void SceneManager::PreDraw()
{
	m_currentScene->PreDraw();
}

void SceneManager::Draw()
{
	m_currentScene->Draw();
}

void SceneManager::DrawSprite()
{
	m_currentScene->DrawSprite();
	KdShaderManager::Instance().m_spriteShader.DrawBox(0, 0, 640, 360, &m_black);
}

void SceneManager::DrawDebug()
{
	m_currentScene->DrawDebug();
}

const std::shared_ptr<Player>& SceneManager::GetPlayer()
{
	return m_currentScene->GetPlayer();
}

const std::shared_ptr<CameraBase>& SceneManager::GetCamera()
{
	return m_currentScene->GetCamera();
}

const std::list<std::shared_ptr<KdGameObject>>& SceneManager::GetObjList()
{
	return m_currentScene->GetObjList();
}

const std::vector<std::shared_ptr<EnemyBase>>& SceneManager::GetEnemyList()
{
	return m_currentScene->GetEnemyList();
}

const std::list<std::shared_ptr<WeaponBase>>& SceneManager::GetWeaponList()
{
	return m_currentScene->GetWeaponList();
}

const std::list<std::shared_ptr<UIBase>>& SceneManager::GetUIList()
{
	return m_currentScene->GetUIList();
}

void SceneManager::SetPlayer(const std::shared_ptr<Player>& player)
{
	m_currentScene->SetPlayer(player);
}

void SceneManager::SetCamera(const std::shared_ptr<CameraBase>& camera)
{
	m_currentScene->SetCamera(camera);
}

void SceneManager::AddObject(const std::shared_ptr<KdGameObject>& obj)
{
	m_currentScene->AddObject(obj);
}

void SceneManager::AddEnemy(const std::shared_ptr<EnemyBase>& enemy)
{
	m_currentScene->AddEnemy(enemy);
}

void SceneManager::AddWeapon(const std::shared_ptr<WeaponBase>& weapon)
{
	m_currentScene->AddWeapon(weapon);
}

void SceneManager::AddUI(const std::shared_ptr<UIBase>& ui)
{
	m_currentScene->AddUI(ui);
}

void SceneManager::ChangeScene(SceneType sceneType)
{
	KdAudioManager::Instance().StopAllSound();
	KdEffekseerManager::GetInstance().StopAllEffect();

	// 現在のシーン情報を更新
	m_currentSceneType = sceneType;

	// 次のシーンを作成し、現在のシーンにする
	switch (sceneType)
	{
	case SceneType::Title:
		m_currentScene = std::make_shared<TitleScene>();
		m_currentScene->Init();
		break;
	case SceneType::Game:
		m_currentScene = std::make_shared<GameScene>();
		m_currentScene->Init();
		break;
	}
}
