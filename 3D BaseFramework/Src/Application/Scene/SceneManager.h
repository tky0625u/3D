#pragma once

class BaseScene;
class StageManager;
class ObjectManager;
class Player;
class EnemyBase;
class EnemyManager;
class WeaponBase;
class UIBase;
class CameraBase;

class SceneManager
{
public :

	// シーン情報
	enum class SceneType
	{
		Title,
		Game,
	};

	void PreUpdate();
	void Update();
	void PostUpdate();

	void PreDraw();
	void Draw();
	void DrawSprite();
	void DrawDebug();

	// スロー状態の切替
	void SlowChange();
	// スロー値
	const float& GetSlow()const;
	// スローフラグ　trueならスロー中
	const bool GetSlowFlg()const;

	// 次のシーンをセット (次のフレームから切り替わる)
	void SetNextScene(SceneType nextScene)
	{
		m_nextSceneType = nextScene;
	}

	// 現在のシーンのオブジェクトリストを取得
	const std::shared_ptr<Player>&                  GetPlayer();
	const std::shared_ptr<CameraBase>&              GetCamera();
	const std::list<std::shared_ptr<KdGameObject>>& GetObjList();
	const std::vector<std::shared_ptr<EnemyBase>>&  GetEnemyList();
	const std::list<std::shared_ptr<WeaponBase>>&   GetWeaponList();
	const std::list<std::shared_ptr<UIBase>>&       GetUIList();
	const std::list<std::shared_ptr<UIBase>>&       GetPlayerUIList();
	const std::list<std::shared_ptr<UIBase>>&       GetEnemyUIList();
	const std::shared_ptr<EnemyManager>&            GetEnemyManager();

	const SceneType GetNowSceneType()const { return m_currentSceneType; }
	
	// 現在のシーンにオブジェクトを追加
	void SetPlayer(const std::shared_ptr<Player>& player);
	void SetCamera(const std::shared_ptr<CameraBase>& camera);
	void AddObject(const std::shared_ptr<KdGameObject>& obj);
	void AddEnemy (const std::shared_ptr<EnemyBase>& enemy);
	void AddWeapon(const std::shared_ptr<WeaponBase>& weapon);
	void AddUI    (const std::shared_ptr<UIBase>& ui);
	void AddPlayerUI    (const std::shared_ptr<UIBase>& ui);
	void AddEnemyUI    (const std::shared_ptr<UIBase>& ui);

	void DebugObject();

	// マネージャーの初期化
	// インスタンス生成(アプリ起動)時にコンストラクタで自動実行
	void Init()
	{
		// 開始シーンに切り替え
		ChangeScene(m_currentSceneType);
	}


	void BlackAlphaChange(float _change, bool _flg)
	{
		m_blackChangeFlg = _flg;

		m_blackChange = _change;
	}
	const bool GetBlackAlphaFlg()const { return m_blackChangeFlg; }
	const float GetBlackAlpha()const { return m_blackAlpha; }

	bool m_stop = false;

private:

	// シーン切り替え関数
	void ChangeScene(SceneType sceneType);

	// 現在のシーンのインスタンスを保持しているポインタ
	std::shared_ptr<BaseScene> m_currentScene = nullptr;

	// 現在のシーンの種類を保持している変数
	SceneType m_currentSceneType = SceneType::Title;
	
	// 次のシーンの種類を保持している変数
	SceneType m_nextSceneType = m_currentSceneType;

	bool  m_blackChangeFlg = false;
	float m_blackChange = 0.0f;
	float m_blackAlpha = 0.0f;
	Math::Color m_black = { 0.0f,0.0f,0.0f,m_blackAlpha };

	// スロー値
	float                                    m_slow = 1.0f;
	// スローフラグ
	bool                                     m_slowFlg = false;

private:

	SceneManager() { }
	~SceneManager() {}

public:

	// シングルトンパターン
	// 常に存在する && 必ず1つしか存在しない(1つしか存在出来ない)
	// どこからでもアクセスが可能で便利だが
	// 何でもかんでもシングルトンという思考はNG
	static SceneManager& Instance()
	{
		static SceneManager instance;
		return instance;
	}
};
