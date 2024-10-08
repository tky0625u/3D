﻿#include "main.h"

#include "Scene/SceneManager.h"
#include"GameObject/ObjectManager.h"
#include"GameObject/Character/Enemy/Bone/Bone.h"
#include"GameObject/Character/Player/Player.h"

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// エントリーポイント
// アプリケーションはこの関数から進行する
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_  HINSTANCE, _In_ LPSTR , _In_ int)
{
	// メモリリークを知らせる
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// COM初期化
	if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
	{
		CoUninitialize();

		return 0;
	}

	// mbstowcs_s関数で日本語対応にするために呼ぶ
	setlocale(LC_ALL, "japanese");

	//===================================================================
	// 実行
	//===================================================================
	Application::Instance().Execute();

	// COM解放
	CoUninitialize();

	return 0;
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// アプリケーション更新開始
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void Application::KdBeginUpdate()
{
	// 入力状況の更新
	KdInputManager::Instance().Update();

	// 空間環境の更新
	KdShaderManager::Instance().WorkAmbientController().Update();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// アプリケーション更新終了
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void Application::KdPostUpdate()
{
	// 3DSoundListnerの行列を更新
	KdAudioManager::Instance().SetListnerMatrix(KdShaderManager::Instance().GetCameraCB().mView.Invert());
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// アプリケーション更新の前処理
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void Application::PreUpdate()
{
	SceneManager::Instance().PreUpdate();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// アプリケーション更新
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void Application::Update()
{
	SceneManager::Instance().Update();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// アプリケーション更新の後処理
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void Application::PostUpdate()
{
	SceneManager::Instance().PostUpdate();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// アプリケーション描画開始
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void Application::KdBeginDraw(bool usePostProcess)
{
	KdDirect3D::Instance().ClearBackBuffer();

	KdShaderManager::Instance().WorkAmbientController().Draw();

	if (!usePostProcess) return;
	KdShaderManager::Instance().m_postProcessShader.Draw();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// アプリケーション描画終了
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void Application::KdPostDraw()
{
	// ImGui処理
	ImGuiProcess();

	// BackBuffer -> 画面表示
	KdDirect3D::Instance().WorkSwapChain()->Present(0, 0);
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// アプリケーション描画の前処理
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void Application::PreDraw()
{
	SceneManager::Instance().PreDraw();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// アプリケーション描画
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void Application::Draw()
{
	SceneManager::Instance().Draw();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// アプリケーション描画の後処理
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void Application::PostDraw()
{
	// 画面のぼかしや被写界深度処理の実施
	KdShaderManager::Instance().m_postProcessShader.PostEffectProcess();

	// 現在のシーンのデバッグ描画
	SceneManager::Instance().DrawDebug();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 2Dスプライトの描画
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void Application::DrawSprite()
{
	SceneManager::Instance().DrawSprite();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// アプリケーション初期設定
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
bool Application::Init(int w, int h)
{
	//===================================================================
	// ウィンドウ作成
	//===================================================================
	if (m_window.Create(w, h, "3D GameProgramming", "Window") == false) {
		MessageBoxA(nullptr, "ウィンドウ作成に失敗", "エラー", MB_OK);
		return false;
	}

	//===================================================================
	// フルスクリーン確認
	//===================================================================
	bool bFullScreen = false;
	/*if (MessageBoxA(m_window.GetWndHandle(), "フルスクリーンにしますか？", "確認", MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDYES) {
		bFullScreen = true;
	}*/

	//===================================================================
	// Direct3D初期化
	//===================================================================

	// デバイスのデバッグモードを有効にする
	bool deviceDebugMode = false;
#ifdef _DEBUG
	deviceDebugMode = true;
#endif

	// Direct3D初期化
	std::string errorMsg;
	if (KdDirect3D::Instance().Init(m_window.GetWndHandle(), w, h, deviceDebugMode, errorMsg) == false) {
		MessageBoxA(m_window.GetWndHandle(), errorMsg.c_str(), "Direct3D初期化失敗", MB_OK | MB_ICONSTOP);
		return false;
	}

	// フルスクリーン設定
	if (bFullScreen) {
		HRESULT hr;

		hr = KdDirect3D::Instance().SetFullscreenState(TRUE, 0);
		if (FAILED(hr))
		{
			MessageBoxA(m_window.GetWndHandle(), "フルスクリーン設定失敗", "Direct3D初期化失敗", MB_OK | MB_ICONSTOP);
			return false;
		}
	}

	//===================================================================
	// ImGui初期化
	//===================================================================
	ImGuiInit();

	//===================================================================
	// シェーダー初期化
	//===================================================================
	KdShaderManager::Instance().Init();

	//===================================================================
	// オーディオ初期化
	//===================================================================
	KdAudioManager::Instance().Init();

	return true;
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// アプリケーション実行
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void Application::Execute()
{
	KdCSVData windowData("Asset/Data/WindowSettings.csv");
	const std::vector<std::string>& sizeData = windowData.GetLine(0);

	//===================================================================
	// 初期設定(ウィンドウ作成、Direct3D初期化など)
	//===================================================================
	if (Application::Instance().Init(atoi(sizeData[0].c_str()), atoi(sizeData[1].c_str())) == false) {
		return;
	}

	//===================================================================
	// ゲームループ
	//===================================================================

	// 時間
	m_fpsController.Init();
	
	SceneManager::Instance();
	SceneManager::Instance().Init();

	ObjectManager::Instance();

	// ループ
	while (1)
	{
		// 処理開始時間Get
		m_fpsController.UpdateStartTime();

		// ゲーム終了指定があるときはループ終了
		if (m_endFlag)
		{
			break;
		}

		//=========================================
		//
		// ウィンドウ関係の処理
		//
		//=========================================

		// ウィンドウのメッセージを処理する
		m_window.ProcessMessage();

		// ウィンドウが破棄されてるならループ終了
		if (m_window.IsCreated() == false)
		{
			break;
		}

		if (GetAsyncKeyState(VK_ESCAPE))
		{
//			if (MessageBoxA(m_window.GetWndHandle(), "本当にゲームを終了しますか？",
//				"終了確認", MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDYES)
			{
				End();
			}
		}
		//=========================================
		//
		// アプリケーション更新処理
		//
		//=========================================

		KdBeginUpdate();
		{
			PreUpdate();

			Update();

			PostUpdate();
		}
		KdPostUpdate();

		//=========================================
		//
		// アプリケーション描画処理
		//
		//=========================================

		KdBeginDraw();
		{
			PreDraw();

			Draw();

			PostDraw();

			DrawSprite();
		}
		KdPostDraw();

		//=========================================
		//
		// フレームレート制御
		//
		//=========================================

		m_fpsController.Update();
	}

	//===================================================================
	// アプリケーション解放
	//===================================================================
	Release();
}

// アプリケーション終了
void Application::Release()
{
	KdInputManager::Instance().Release();

	KdShaderManager::Instance().Release();

	KdAudioManager::Instance().Release();

	ImGuiRelease();	// ImGui解放処理

	KdDirect3D::Instance().Release();

	// ウィンドウ削除
	m_window.Release();
}

void Application::ImGuiInit()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	// Setup Dear ImGui style
	// ImGui::StyleColorsDark();
	ImGui::StyleColorsClassic();
	// Setup Platform/Renderer bindings
ImGui_ImplWin32_Init(m_window.GetWndHandle());
ImGui_ImplDX11_Init(
	KdDirect3D::Instance().WorkDev(), KdDirect3D::Instance().WorkDevContext());

#include "imgui/ja_glyph_ranges.h"
ImGuiIO& io = ImGui::GetIO();
ImFontConfig config;
config.MergeMode = true;
io.Fonts->AddFontDefault();
// 日本語対応
io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\msgothic.ttc", 13.0f, &config, glyphRangesJapanese);
}

void Application::ImGuiProcess()
{
	//return;

	//===========================================================
	// ImGui開始
	//===========================================================
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//===========================================================
	// 以下にImGui描画処理を記述
	//===========================================================

	// デバッグウィンドウ
	if (ImGui::Begin("Debug Window"))
	{
		// FPS
		//ImGui::Text("FPS : %d", m_fpsController.m_nowfps);

		static bool stop = false;
		ImGui::Checkbox((const char*)u8"ストップ", &stop);
		SceneManager::Instance().m_stop = stop;

		if (ImGui::TreeNode("Player"))
		{
			if (ObjectManager::Instance().GetPlayer().expired() == false)
			{
				std::shared_ptr<Player> _player = ObjectManager::Instance().GetPlayer().lock();

				// 体力
				ImGui::Text((const char*)u8"　体力 　　HP=%d", _player->GetParam().Hp);
				int hp = _player->GetParam().Hp;
				ImGui::SliderInt("HP%d", &hp, 1, 100);
				// 攻撃力
				ImGui::Text((const char*)u8"　攻撃力 　ATK=%d", _player->GetParam().Atk);
				int atk = _player->GetParam().Atk;
				ImGui::SliderInt("ATK", &atk, 1, 100);
				// 素早さ
				ImGui::Text((const char*)u8"　素早さ 　SP=%.2f", _player->GetParam().Sp);
				float speed = _player->GetParam().Sp;
				ImGui::SliderFloat("Speed", &speed, 1, 100);
				// スタミナ
				ImGui::Text((const char*)u8"　スタミナ SM=%d", _player->GetParam().Sm);
				int stamina = _player->GetParam().Sm;
				ImGui::SliderInt("Stamina", &stamina, 1, 100);
				// 位置
				ImGui::Text((const char*)u8"　位置 　　x=%.2f,y=%.2f,z=%.2f", _player->GetParam().Pos.x, _player->GetParam().Pos.y, _player->GetParam().Pos.z);
				Math::Vector3 pos = _player->GetParam().Pos;
				ImGui::SliderFloat("PosX", &pos.x, -100, 100);
				ImGui::SliderFloat("PosY", &pos.y, 0, 100);
				ImGui::SliderFloat("PosZ", &pos.z, -100, 100);
				// 方向
				ImGui::Text((const char*)u8"　方向 　　x=%.2f,y=%.2f,z=%.2f", _player->GetParam().Dir.x, _player->GetParam().Dir.y, _player->GetParam().Dir.z);
				// 角度
				ImGui::Text((const char*)u8"　角度 　　Angle=%.2f", _player->GetParam().Angle);
				float angle = _player->GetParam().Angle;
				ImGui::SliderFloat("Angle", &angle, 0, 360);
				// 大きさ
				ImGui::Text((const char*)u8"　大きさ 　Size=%.2f,y=%.2f,z=%.2f", _player->GetParam().Size);
				float size = _player->GetParam().Size;
				ImGui::SliderFloat("Size", &size, 1, 100);
				// 攻撃範囲
				ImGui::Text((const char*)u8"　攻撃範囲 ATKRange=%.2f", _player->GetParam().AtkRange);
				float range = _player->GetParam().AtkRange;
				ImGui::SliderFloat("ATKRange", &range, 1, 100);
				// 前方方向
				ImGui::Text((const char*)u8"　前方方向 x=%.2f,y=%.2f,z=%.2f", _player->GetParam().ForwardX, _player->GetParam().ForwardY, _player->GetParam().ForwardZ);
				// 無敵時間
				ImGui::Text((const char*)u8"　無敵付与時間 InviTime=%d", _player->GetinviTime());
				int _inviTime = _player->GetinviTime();
				ImGui::SliderInt("InviTIme", &_inviTime, 0, 300);

				_player->SetParam(hp, atk, speed, stamina, pos, _player->GetParam().Dir, angle, size, range, Math::Vector3{ _player->GetParam().ForwardX, _player->GetParam().ForwardY, _player->GetParam().ForwardZ });
				_player->SetInviTime(_inviTime);

				ImGui::Text((const char*)u8"------------------------------------------------------------------------------------------------------------------------------------------------------------------");

				ImGui::TreePop();
			}
		}

		if (ImGui::TreeNode("Enemy"))
		{
			if (ImGui::Button((const char*)u8"Enemy保存"))
			{
				ObjectManager::Instance().EnemyWrite();
			}

			if (ImGui::TreeNode("Bone"))
			{
				if (ImGui::Button((const char*)u8"Bone追加"))
				{
					ObjectManager::Instance().AddBone();
				}

				static int operation = -1;
				if (!SceneManager::Instance().m_stop)operation = -1;

				if (ImGui::TreeNode("Bone"))
				{
					std::vector<std::weak_ptr<Bone>> _boneList = ObjectManager::Instance().GetBoneList();
					ImGui::Text((const char*)u8"ボーン:%d体", _boneList.size());
					for (int bone = 0; bone < ObjectManager::Instance().GetBoneList().size(); ++bone)
					{
						if (ImGui::Button(std::to_string(bone + 1).c_str()))
						{
							operation = bone;
						}
					}

					if (operation!=-1)
					{
						ImGui::Text((const char*)u8"%d体目", operation + 1);
						// 体力
						ImGui::Text((const char*)u8"　体力 　　HP=%d", _boneList[operation].lock()->GetParam().Hp);
						int hp = _boneList[operation].lock()->GetParam().Hp;
						ImGui::SliderInt("HP%d", &hp, 1, 100);
						// 攻撃力
						ImGui::Text((const char*)u8"　攻撃力 　ATK=%d", _boneList[operation].lock()->GetParam().Atk);
						int atk = _boneList[operation].lock()->GetParam().Atk;
						ImGui::SliderInt("ATK", &atk, 1, 100);
						// 素早さ
						ImGui::Text((const char*)u8"　素早さ 　SP=%.2f", _boneList[operation].lock()->GetParam().Sp);
						float speed = _boneList[operation].lock()->GetParam().Sp;
						ImGui::SliderFloat("Speed", &speed, 1, 100);
						// スタミナ
						ImGui::Text((const char*)u8"　スタミナ SM=%d", _boneList[operation].lock()->GetParam().Sm);
						int stamina = _boneList[operation].lock()->GetParam().Sm;
						ImGui::SliderInt("Stamina", &stamina, 1, 100);
						// 位置
						ImGui::Text((const char*)u8"　位置 　　x=%.2f,y=%.2f,z=%.2f", _boneList[operation].lock()->GetParam().Pos.x, _boneList[operation].lock()->GetParam().Pos.y, _boneList[operation].lock()->GetParam().Pos.z);
						Math::Vector3 pos = _boneList[operation].lock()->GetParam().Pos;
						ImGui::SliderFloat("PosX", &pos.x, -100, 100);
						ImGui::SliderFloat("PosY", &pos.y, 0, 100);
						ImGui::SliderFloat("PosZ", &pos.z, -100, 100);
						// 方向
						ImGui::Text((const char*)u8"　方向 　　x=%.2f,y=%.2f,z=%.2f", _boneList[operation].lock()->GetParam().Dir.x, _boneList[operation].lock()->GetParam().Dir.y, _boneList[operation].lock()->GetParam().Dir.z);
						// 角度
						ImGui::Text((const char*)u8"　角度 　　Angle=%.2f", _boneList[operation].lock()->GetParam().Angle);
						float angle = _boneList[operation].lock()->GetParam().Angle;
						ImGui::SliderFloat("Angle", &angle, 0, 360);
						// 大きさ
						ImGui::Text((const char*)u8"　大きさ 　Size=%.2f", _boneList[operation].lock()->GetParam().Size);
						float size = _boneList[operation].lock()->GetParam().Size;
						ImGui::SliderFloat("Size", &size, 1, 100);
						// 攻撃範囲
						ImGui::Text((const char*)u8"　攻撃範囲 ATKRange=%.2f", _boneList[operation].lock()->GetParam().AtkRange);
						float range = _boneList[operation].lock()->GetParam().AtkRange;
						ImGui::SliderFloat("ATKRange", &range, 1, 100);
						// 前方方向
						ImGui::Text((const char*)u8"　前方方向 x=%.2f,y=%.2f,z=%.2f", _boneList[operation].lock()->GetParam().ForwardX, _boneList[operation].lock()->GetParam().ForwardY, _boneList[operation].lock()->GetParam().ForwardZ);

						_boneList[operation].lock()->SetParam(hp, atk, speed, stamina, pos, _boneList[operation].lock()->GetParam().Dir, angle, size, range, Math::Vector3{ _boneList[operation].lock()->GetParam().ForwardX, _boneList[operation].lock()->GetParam().ForwardY, _boneList[operation].lock()->GetParam().ForwardZ });

						if (ImGui::Button((const char*)u8"消滅"))
						{
							_boneList[operation].lock()->Expired();
							operation = -1;
						}

						ImGui::Text((const char*)u8"------------------------------------------------------------------------------------------------------------------------------------------------------------------");
					}
						ImGui::TreePop();
				}

				ImGui::TreePop();
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode((const char*)u8"シーン切替"))
		{
			if (ImGui::Button("Title"))
			{
				SceneManager::Instance().SetNextScene(SceneManager::SceneType::Title);
			}
			if (ImGui::Button("Game"))
			{
				SceneManager::Instance().SetNextScene(SceneManager::SceneType::Game);
			}

			ImGui::TreePop();
		}
	}
	ImGui::End();

	// ログウィンドウ
	m_log.Draw("Log Window");

	//=====================================================
	// ログ出力 ・・・ AddLog("～") で追加
	//=====================================================
	
	//m_log.AddLog("hello world\n");

	//=====================================================
	// 別ソースファイルからログを出力する場合
	//=====================================================

	// "main.h"のインクルード必須
	//Application::Instance().m_log.AddLog("TestLog\n");

	//===========================================================
	// ここより上にImGuiの描画はする事
	//===========================================================
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void Application::ImGuiRelease()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
