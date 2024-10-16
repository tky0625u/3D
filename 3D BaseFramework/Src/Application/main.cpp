#include "main.h"

#include"Scene/SceneManager.h"
#include"GameObject/ObjectManager.h"
#include"GameObject/Character/Enemy/Bone/Bone.h"
#include"GameObject/Character/Player/Player.h"
#include"GameObject/Weapon/Sword/Sword.h"
#include"GameObject/Weapon/Shield/Shield.h"
#include"GameObject/SkyBox/SkyBox.h"
#include"GameObject/Stage/Ground/Ground.h"
#include"GameObject/Stage/Wall/Wall.h"

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

		switch (SceneManager::Instance().GetNowSceneType())
		{
		case SceneManager::SceneType::Title:

			break;
		case SceneManager::SceneType::Game:

			if (ImGui::TreeNode("Player"))
			{
				if (ImGui::Button((const char*)u8"Player保存"))
				{
					ObjectManager::Instance().PlayerWrite();
				}

				if (ObjectManager::Instance().GetPlayer().expired() == false)
				{
					std::shared_ptr<Player> _player = ObjectManager::Instance().GetPlayer().lock();

					// 体力
					ImGui::Text((const char*)u8"　体力 　　HP=%d", _player->GetParam().Hp);
					int hp = _player->GetParam().Hp;
					ImGui::SliderInt("HP", &hp, 1, 100);
					// 攻撃力
					ImGui::Text((const char*)u8"　攻撃力 　ATK=%d", _player->GetParam().Atk);
					// 素早さ
					ImGui::Text((const char*)u8"　素早さ 　SP=%.2f", _player->GetParam().Sp);
					float speed = _player->GetParam().Sp;
					ImGui::SliderFloat("Speed", &speed, 1, 100);
					// スタミナ
					ImGui::Text((const char*)u8"　スタミナ SM=%d", _player->GetParam().Sm);
					int stamina = _player->GetParam().Sm;
					ImGui::SliderInt("Stamina", &stamina, 1, 100);
					// 位置
					ImGui::Text((const char*)u8"　位置 　　x=%.2f,y=%.2f,z=%.2f", _player->GetPos().x, _player->GetPos().y, _player->GetPos().z);
					Math::Vector3 pos = _player->GetPos();
					ImGui::SliderFloat("PosX", &pos.x, -100, 100);
					ImGui::SliderFloat("PosY", &pos.y, 0, 400);
					ImGui::SliderFloat("PosZ", &pos.z, -100, 100);
					// 方向
					ImGui::Text((const char*)u8"　方向 　　x=%.2f,y=%.2f,z=%.2f", _player->GetDir().x, _player->GetDir().y, _player->GetDir().z);
					// 角度
					ImGui::Text((const char*)u8"　角度 　　Angle=%.2f", _player->GetAngle());
					float angle = _player->GetAngle();
					ImGui::SliderFloat("Angle", &angle, 0, 360);
					// 大きさ
					ImGui::Text((const char*)u8"　大きさ 　Size=%.2f", _player->GetSize());
					float size = _player->GetSize();
					ImGui::SliderFloat("Size", &size, 1, 100);
					// 攻撃範囲
					ImGui::Text((const char*)u8"　攻撃範囲 ATKRange=%.2f", _player->GetAtkRange());
					float range = _player->GetAtkRange();
					ImGui::SliderFloat("ATKRange", &range, 1, 100);
					// 前方方向
					ImGui::Text((const char*)u8"　前方方向 x=%.2f,y=%.2f,z=%.2f", _player->GetForward().x, _player->GetForward().y, _player->GetForward().z);
					// 無敵時間
					ImGui::Text((const char*)u8"　無敵付与時間 InviTime=%d", _player->GetinviTime());
					int _inviTime = _player->GetinviTime();
					ImGui::SliderInt("InviTIme", &_inviTime, 0, 300);

					// 武器
					static std::string _swordName = _player->GetSword().lock()->GetName().c_str();
					static std::string _shieldName = _player->GetShield().lock()->GetName().c_str();
					if (ImGui::TreeNode("Weapon"))
					{
						// 剣
						if (_player->GetSword().expired() == false)
						{
							std::shared_ptr<Sword> _sword = _player->GetSword().lock();
							static Math::Vector3 swordPos = _sword->GetPos();
							static int swordAtk = _sword->GetATK();
							static float swordSize = _sword->GetSize();
							static int traje = _sword->GetTraject();
							if (ImGui::TreeNode("Sword"))
							{
								if (ImGui::Button((const char*)u8"Sword保存"))
								{
									ObjectManager::Instance().SwordWrite(_swordName);
								}

								ImGui::Text((const char*)u8"　剣 Sword=%s", _player->GetSword().lock()->GetName().c_str());
								for (auto& sword : ObjectManager::Instance().GetSwordNameList())
								{
									if (ImGui::Button(sword.c_str()))
									{
										_swordName = sword;
									}
								}

								// 位置
								ImGui::Text((const char*)u8"　位置 　　x=%.2f,y=%.2f,z=%.2f", _sword->GetPos().x, _sword->GetPos().y, _sword->GetPos().z);
								swordPos = _sword->GetPos();
								ImGui::SliderFloat("PosX", & swordPos.x, -0.5, 0.5);
								ImGui::SliderFloat("PosY", & swordPos.y, -0.5, 0.5);
								ImGui::SliderFloat("PosZ", & swordPos.z, -0.5, 0.5);
								// 攻撃力
								ImGui::Text((const char*)u8"　攻撃力 　ATK=%d", _sword->GetATK());
								swordAtk = _sword->GetATK();
								ImGui::SliderInt("ATK", &swordAtk, 1, 100);
								// 大きさ
								ImGui::Text((const char*)u8"　大きさ 　Size=%.2f", _sword->GetSize());
								swordSize = _sword->GetSize();
								ImGui::SliderFloat("Size", &swordSize, 1, 100);
								// トレイルポリゴン
								ImGui::Text((const char*)u8"　トレイルポリゴン　Traject=%d", _sword->GetTraject());
								traje = _sword->GetTraject();
								ImGui::SliderInt("Traject", &traje, 1, 100);

								_sword->SetPos(swordPos);
								_sword->SetATK(swordAtk);
								_sword->SetSize(swordSize);
								_sword->SetTrajectPointNUM(traje);

								ImGui::TreePop();
							}
						}

						// 盾
						if (_player->GetShield().expired() == false)
						{
							if (ImGui::TreeNode("Shield"))
							{
								std::shared_ptr<Shield> _shield = _player->GetShield().lock();
								static Math::Vector3 shieldPos = _shield->GetPos();
								static float shieldSize = _shield->GetSize();
								if (ImGui::Button((const char*)u8"Shield保存"))
								{
									ObjectManager::Instance().ShieldWrite(_shieldName);
								}

								ImGui::Text((const char*)u8"　盾 Shield=%s", _player->GetShield().lock()->GetName().c_str());
								for (auto& shield : ObjectManager::Instance().GetShieldNameList())
								{
									if (ImGui::Button(shield.c_str()))
									{
										_shieldName = shield;
									}
								}

								// 位置
								ImGui::Text((const char*)u8"　位置 　　x=%.2f,y=%.2f,z=%.2f", _shield->GetPos().x, _shield->GetPos().y, _shield->GetPos().z);
								shieldPos = _shield->GetPos();
								ImGui::SliderFloat("PosX", &shieldPos.x, -0.5, 0.5);
								ImGui::SliderFloat("PosY", &shieldPos.y, -0.5, 0.5);
								ImGui::SliderFloat("PosZ", &shieldPos.z, -0.5, 0.5);

								// 大きさ
								ImGui::Text((const char*)u8"　大きさ 　Size=%.2f", _shield->GetSize());
								shieldSize = _shield->GetSize();
								ImGui::SliderFloat("Size", &shieldSize, 1, 100);

								_shield->SetPos(shieldPos);
								_shield->SetSize(shieldSize);

								ImGui::TreePop();
							}
						}

						ImGui::TreePop();

						ObjectManager::Instance().ChangeWeapon(_swordName, _shieldName);
					}

					_player->SetParam(hp, _player->GetSword().lock()->GetATK(), speed, stamina);
					_player->SetPos(pos);
					_player->SetAngle(angle);
					_player->SetSize(size);
					_player->SetAtkRange(range);
					_player->SetInviTime(_inviTime);

					ImGui::Text((const char*)u8"------------------------------------------------------------------------------------------------------------------------------------------------------------------");

					ImGui::TreePop();
				}
			}

			if (ImGui::TreeNode("Enemy"))
			{
				std::vector<std::weak_ptr<EnemyBase>> _boneList;
				for (auto& enemy : ObjectManager::Instance().GetEnemyList())
				{
					if (enemy.expired() == false)
					{
						if (enemy.lock()->GetName() == "Bone")
						{
							_boneList.push_back(enemy);
						}
					}
				}

				static int _stageNum = 1;
				ImGui::Text((const char*)u8"ステージ数 : %d", _stageNum);
				static int _nowStage = ObjectManager::Instance().GetnowWave();
				ImGui::SliderInt((const char*)u8"ステージ", &_nowStage, 1, _stageNum);
				if (ImGui::Button((const char*)u8"ステージ追加"))
				{
					_stageNum++;
					if (_stageNum - 1 == _nowStage)_nowStage = _stageNum;
				}

				static int _wave = ObjectManager::Instance().GetMaxWave();
				ImGui::Text((const char*)u8"ウェーブ数 : %d", _wave);
				static int _nowWave = ObjectManager::Instance().GetnowWave();
				ImGui::SliderInt((const char*)u8"ウェーブ", &_nowWave, 1, _wave);
				if (ImGui::Button((const char*)u8"ウェーブ追加"))
				{
					_wave++;
					if (_wave - 1 == _nowWave)_nowWave = _wave;
				}
				if (ImGui::Button((const char*)u8"ウェーブ削除"))
				{
					if (_wave > 1)
					{
						_wave--;
						if (_wave + 1 == _nowWave)_nowWave = _wave;
					}
				}

				if (ImGui::Button((const char*)u8"Enemy保存"))
				{
					ObjectManager::Instance().EnemyWrite(_nowStage, _nowWave);
					if (_nowWave == _wave)
					{
						_wave++;
						_nowWave = _wave;
					}
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
						ImGui::Text((const char*)u8"ボーン:%d体", _boneList.size());
						for (int bone = 0; bone <_boneList.size(); ++bone)
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
							ImGui::SliderInt("HP", &hp, 1, 100);
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
							ImGui::Text((const char*)u8"　位置 　　x=%.2f,y=%.2f,z=%.2f", _boneList[operation].lock()->GetPos().x, _boneList[operation].lock()->GetPos().y, _boneList[operation].lock()->GetPos().z);
							Math::Vector3 pos = _boneList[operation].lock()->GetPos();
							ImGui::SliderFloat("PosX", &pos.x, -100, 100);
							ImGui::SliderFloat("PosY", &pos.y, 0, 400);
							ImGui::SliderFloat("PosZ", &pos.z, -100, 100);
							// 方向
							ImGui::Text((const char*)u8"　方向 　　x=%.2f,y=%.2f,z=%.2f", _boneList[operation].lock()->GetDir().x, _boneList[operation].lock()->GetDir().y, _boneList[operation].lock()->GetDir().z);
							// 角度
							ImGui::Text((const char*)u8"　角度 　　Angle=%.2f", _boneList[operation].lock()->GetAngle());
							float angle = _boneList[operation].lock()->GetAngle();
							ImGui::SliderFloat("Angle", &angle, 0, 360);
							// 大きさ
							ImGui::Text((const char*)u8"　大きさ 　Size=%.2f", _boneList[operation].lock()->GetSize());
							float size = _boneList[operation].lock()->GetSize();
							ImGui::SliderFloat("Size", &size, 1, 100);
							// 攻撃範囲
							ImGui::Text((const char*)u8"　攻撃範囲 ATKRange=%.2f", _boneList[operation].lock()->GetAtkRange());
							float range = _boneList[operation].lock()->GetAtkRange();
							ImGui::SliderFloat("ATKRange", &range, 1, 100);
							// 前方方向
							ImGui::Text((const char*)u8"　前方方向 x=%.2f,y=%.2f,z=%.2f", _boneList[operation].lock()->GetForward().x, _boneList[operation].lock()->GetForward().y, _boneList[operation].lock()->GetForward().z);

							_boneList[operation].lock()->SetParam(hp, atk, speed, stamina);
							_boneList[operation].lock()->SetPos(pos);
							_boneList[operation].lock()->SetAngle(angle);
							_boneList[operation].lock()->SetSize(size);
							_boneList[operation].lock()->SetAtkRange(range);

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

			if (ImGui::TreeNode("Object"))
			{
				if (ImGui::Button((const char*)u8"Object保存"))ObjectManager::Instance().ObjectWrite();
				static int Goperation = -1;
				static int Coperation = -1;
				static int Woperation = -1;

				std::vector<std::weak_ptr<KdGameObject>> groundList;
				std::vector<std::weak_ptr<KdGameObject>> circleList;
				std::vector<std::weak_ptr<KdGameObject>> magicList;
				std::vector<std::weak_ptr<KdGameObject>> wallList;
				std::vector<std::weak_ptr<KdGameObject>> skyboxList;
				for (auto& obj : ObjectManager::Instance().GetObjectList())
				{
					if (obj.expired() == false)
					{
						if (obj.lock()->GetName() == "Ground")
						{
							groundList.push_back(obj);
						}
						else if (obj.lock()->GetName() == "Circle")
						{
							circleList.push_back(obj);
						}
						else if (obj.lock()->GetName() == "Magic")
						{
							magicList.push_back(obj);
						}
						else if (obj.lock()->GetName() == "Wall")
						{
							wallList.push_back(obj);
						}
						else if (obj.lock()->GetName() == "SkyBox")
						{
							skyboxList.push_back(obj);
						}
					}
				}

				if (ImGui::TreeNode("Ground"))
				{
					if (ImGui::Button((const char*)u8"Ground追加"))ObjectManager::Instance().AddGround();
					for (int g = 0; g < groundList.size(); ++g)
					{
						if (ImGui::Button(std::to_string(g + 1).c_str()))Goperation = g;
					}

					if (Goperation != -1)
					{
						ImGui::Text((const char*)u8"%d個目", Goperation + 1);
						// 位置
						ImGui::Text((const char*)u8"　位置 　　x=%.2f,y=%.2f,z=%.2f", groundList[Goperation].lock()->GetPos().x, groundList[Goperation].lock()->GetPos().y, groundList[Goperation].lock()->GetPos().z);
						Math::Vector3 pos = groundList[Goperation].lock()->GetPos();
						ImGui::SliderFloat("PosX", &pos.x, -100, 100);
						ImGui::SliderFloat("PosY", &pos.y, 0, 500);
						ImGui::SliderFloat("PosZ", &pos.z, -100, 100);
						// 角度
						ImGui::Text((const char*)u8"　角度 　　Angle=%.2f", groundList[Goperation].lock()->GetAngle());
						float angle = groundList[Goperation].lock()->GetAngle();
						ImGui::SliderFloat("Angle", &angle, 0, 360);
						// 大きさ
						ImGui::Text((const char*)u8"　大きさ 　Size=%.2f", groundList[Goperation].lock()->GetSize());
						float size = groundList[Goperation].lock()->GetSize();
						ImGui::SliderFloat("Size", &size, 1, 100);
						
						groundList[Goperation].lock()->SetPos(pos);
						groundList[Goperation].lock()->SetSize(size);
						groundList[Goperation].lock()->SetAngle(angle);

						if (ImGui::Button((const char*)u8"消滅"))
						{
							groundList[Goperation].lock()->Expired();
							if (circleList.size() == groundList.size())
							{
								circleList[Goperation].lock()->Expired();
								magicList[Goperation].lock()->Expired();
							}
							ObjectManager::Instance().DeleteObjectList();
							Goperation = -1;
							Coperation = -1;
						}

						ImGui::Text((const char*)u8"------------------------------------------------------------------------------------------------------------------------------------------------------------------");
					}
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Circle"))
				{
					if (ImGui::Button((const char*)u8"Circle追加"))
					{
						if (groundList.size() > circleList.size())ObjectManager::Instance().AddCircle();
					}

					if (circleList.size() != 0)
					{
						// 角度
						ImGui::Text((const char*)u8"　角度 　　Angle=%.2f", circleList[0].lock()->GetAngle());
						float angle = circleList[0].lock()->GetAngle();
						ImGui::SliderFloat("Angle", &angle, 0, 360);
						// 大きさ
						ImGui::Text((const char*)u8"　大きさ 　Size=%.2f", circleList[0].lock()->GetSize());
						float size = circleList[0].lock()->GetSize();
						ImGui::SliderFloat("Size", &size, 1, 100);

						for (int c = 0; c < circleList.size(); ++c)
						{
							circleList[c].lock()->SetSize(size);
							circleList[c].lock()->SetAngle(angle);

							if (ImGui::Button(std::to_string(c + 1).c_str()))Coperation = c;
						}

						if (Coperation != -1)
						{
							if (circleList[Coperation].expired() == false)
							{
								ImGui::Text((const char*)u8"%d個目", Coperation + 1);
								// 位置
								Math::Vector3 pos;
								ImGui::Text((const char*)u8"　位置 　　x=%.2f,y=%.2f,z=%.2f", circleList[Coperation].lock()->GetPos().x, circleList[Coperation].lock()->GetPos().y, circleList[Coperation].lock()->GetPos().z);
								pos = circleList[Coperation].lock()->GetPos();
								if (groundList[Coperation].expired() == false)pos.y = groundList[Coperation].lock()->GetPos().y;
								ImGui::SliderFloat("PosX", &pos.x, -100, 100);
								ImGui::SliderFloat("PosZ", &pos.z, -100, 100);

								circleList[Coperation].lock()->SetPos(pos);

								if (ImGui::Button((const char*)u8"消滅"))
								{
									circleList[Coperation].lock()->Expired();
									magicList[Coperation].lock()->Expired();
									ObjectManager::Instance().DeleteObjectList();
									Coperation = -1;
								}
							}
						}


						ImGui::Text((const char*)u8"------------------------------------------------------------------------------------------------------------------------------------------------------------------");

						if (ImGui::TreeNode("Magic"))
						{
							float size = 0.0f;
							if (magicList[0].expired() == false)
							{
								// 大きさ
								ImGui::Text((const char*)u8"　大きさ 　Size=%.2f", magicList[0].lock()->GetSize());
								size = magicList[0].lock()->GetSize();
								ImGui::SliderFloat("Size", &size, 1, 100);
							}

							for (int m = 0; m < magicList.size(); ++m)
							{
								if (circleList[m].expired() == false)
								{
									magicList[m].lock()->SetSize(size);
								}
							}
							ImGui::TreePop();
						}
					}

					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Wall"))
				{
					if (ImGui::Button((const char*)u8"Wall追加"))ObjectManager::Instance().AddWall();
					for (int w = 0; w < wallList.size(); ++w)
					{
						if (ImGui::Button(std::to_string(w + 1).c_str()))Woperation = w;
					}

					if (Woperation != -1)
					{
						ImGui::Text((const char*)u8"%d個目", Woperation + 1);
						// 位置
						ImGui::Text((const char*)u8"　位置 　　x=%.2f,y=%.2f,z=%.2f", wallList[Woperation].lock()->GetPos().x, wallList[Woperation].lock()->GetPos().y, wallList[Woperation].lock()->GetPos().z);
						Math::Vector3 pos = wallList[Woperation].lock()->GetPos();
						ImGui::SliderFloat("PosX", &pos.x, -100, 100);
						ImGui::SliderFloat("PosY", &pos.y, 0, 500);
						ImGui::SliderFloat("PosZ", &pos.z, -100, 100);
						// 角度
						ImGui::Text((const char*)u8"　角度 　　Angle=%.2f", wallList[Woperation].lock()->GetAngle());
						float angle = wallList[Woperation].lock()->GetAngle();
						ImGui::SliderFloat("Angle", &angle, 0, 360);
						// 大きさ
						ImGui::Text((const char*)u8"　大きさ 　Size=%.2f", wallList[Woperation].lock()->GetSize());
						float size = wallList[Woperation].lock()->GetSize();
						ImGui::SliderFloat("Size", &size, 1, 100);

						wallList[Woperation].lock()->SetPos(pos);
						wallList[Woperation].lock()->SetSize(size);
						wallList[Woperation].lock()->SetAngle(angle);

						if (ImGui::Button((const char*)u8"消滅"))
						{
							wallList[Woperation].lock()->Expired();
							ObjectManager::Instance().DeleteObjectList();
							Woperation = -1;
						}

						ImGui::Text((const char*)u8"------------------------------------------------------------------------------------------------------------------------------------------------------------------");
					}
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("SkyBox"))
				{
					// 位置
					ImGui::Text((const char*)u8"　位置 　　x=%.2f,y=%.2f,z=%.2f", skyboxList[0].lock()->GetPos().x, skyboxList[0].lock()->GetPos().y, skyboxList[0].lock()->GetPos().z);
					Math::Vector3 pos = skyboxList[0].lock()->GetPos();
					ImGui::SliderFloat("PosX", &pos.x, -500, 500);
					ImGui::SliderFloat("PosY", &pos.y, 0, 500);
					ImGui::SliderFloat("PosZ", &pos.z, -500, 500);
					// 角度
					ImGui::Text((const char*)u8"　角度 　　Angle=%.2f", skyboxList[0].lock()->GetAngle());
					float angle = skyboxList[0].lock()->GetAngle();
					ImGui::SliderFloat("Angle", &angle, 0, 360);
					// 大きさ
					ImGui::Text((const char*)u8"　大きさ 　Size=%.2f", skyboxList[0].lock()->GetSize());
					float size = skyboxList[0].lock()->GetSize();
					ImGui::SliderFloat("Size", &size, 1, 100);

					skyboxList[0].lock()->SetPos(pos);
					skyboxList[0].lock()->SetSize(size);
					skyboxList[0].lock()->SetAngle(angle);

					ImGui::Text((const char*)u8"------------------------------------------------------------------------------------------------------------------------------------------------------------------");
					ImGui::TreePop();
				}

				ImGui::TreePop();
			}
			break;
		default:
			break;
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
