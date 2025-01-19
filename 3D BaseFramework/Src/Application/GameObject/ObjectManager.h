#pragma once

// ステージマネジャ
class StageManager;
// タイトルカメラ
class TitleCamera;
// ゲームカメラ
class GameCamera;
// タイトルUI
class Title;
// GameUI
class Game;
// ExitUI
class Exit;
// カーソル
class Cursor;
// セレクトUI
class Select;
// 階層UI
class Floor;
// ゲーム状態UI
class GameStateUI;
// プレイヤー
class Player;
// プレイヤーHP
class Player_HP;
// プレイヤースタミナ
class Player_Stamina;
// 敵基底
class EnemyBase;
// 骨
class Bone;
// 骨色違い
class BoneAlpha;
// ゴーレム
class Golem;
// ゴーレム用弾
class Bullet;
// 敵HP
class Enemy_HP;
// 地面
class Ground;
// 魔法陣の台
class Circle;
// 魔法陣
class MagicPolygon;
// 壁
class Wall;
// 空
class SkyBox;

#include"../tinygltf/json.hpp"
class ObjectManager :public std::enable_shared_from_this<ObjectManager>
{
public:
	ObjectManager() {};
	~ObjectManager() {};

	// 今がどのシーンかを確認する
	void SceneCheck();
	
	// 消滅したら配列から削除
	void DeleteEnemyList();
	
	// スロー状態の切替
	void SlowChange();

	// ワープ解放時の演出の準備
	void NextStageLiberation();

	// ゲームクリア
	void GameClear();

	// 次のステージを生成
	void CreateStage(std::shared_ptr<StageManager> _stage);

	// ImGuiで動的にオブジェクトの設定をする
	void DebugObject(std::shared_ptr<StageManager> _stage = nullptr);


	// Jsonファイルに保存======================================================
	// タイトルカメラ
	void TitleCameraWrite();
	// タイトルUI
	void TitleWrite();
	// GameUI
	void GameWrite();
	// ExitUI
	void ExitWrite();
	// セレクトUI
	void SelectWrite();
	// カーソル
	void CursorWrite();
	// カーソルパーティクル
	void CursorParticleWrite();
	// ゲームカメラ
	void GameCameraWrite();
	// プレイヤー
	void PlayerWrite(std::string _fileName);
	// プレイヤーHP
	void PlayerHPWrite();
	// プレイヤースタミナ
	void PlayerStaminaWrite();
	// 階層UI
	void FloorWrite();
	// ゲーム状態UI
	void GameStateWrite();
	// 敵
	void EnemyWrite(int _stage, int _wave, std::string _fileName);
	// 敵HP
	void EnemyHPWrite();
	// 剣
	void SwordWrite(std::string _swordName, std::string _fileName);
	// 盾
	void ShieldWrite(std::string _shieldName, std::string _fileName);
	// オブジェクト
	void ObjectWrite(std::string _fileName);
	//=========================================================================

	// モデルを事前に読み込む
	void ModelLoad();

	// オブジェクトの生成======================================================
	// タイトルカメラ
	void SetTitleCamera();
	// タイトルUI
	void SetTitleParam();
	// GameUI
	void SetGameParam();
	// ExitUI
	void SetExitParam();
	// セレクトUI
	void SetSelectParam();
	// カーソル
	void SetCursorParam();
	// カーソルパーティクル
	void SetCursorParticleParam();
	// ゲームカメラ
	void SetGameCameraParam(std::shared_ptr<StageManager> _stage);
	// オブジェクト
	void SetObjectParam(std::shared_ptr<StageManager> _stage = nullptr);
	// プレイヤー
	void SetPlayerParam(std::shared_ptr<StageManager> _stage);
	// プレイヤーUI
	void SetPlayerUI(std::shared_ptr<StageManager> _stage);
	// プレイヤーHP
	void SetPlayerHPParam();
	// プレイヤースタミナ
	void SetPlayerStaminaParam();
	// 階層UI
	void SetFloorParam(std::shared_ptr<StageManager> _stage);
	// ゲーム状態UI
	void SetGameStateParam(bool _IsClear);
	// 武器
	void SetWeaponParam(std::string _filePath, std::string _weaponName);
	// 敵
	void SetEnemyParam(std::string _filePath = "none", std::shared_ptr<StageManager> _stage = nullptr);
	// 敵HP
	void SetEnemyHPParam(std::shared_ptr<EnemyBase> _enemy);
	// ゴーレム用弾
	std::shared_ptr<Bullet> SetBulletParam();
	// 骨色違い用弾
	void SetBoneAlphaBulletParam(int id);
	//=========================================================================

	// テレポート位置からのフラグ trueならテレポートが可能
	const bool GetTeleportFlg();
	// ゴーレム
	const std::weak_ptr<Golem>& GetGolem()const { return m_golem; }
	// 魔法陣
	const std::weak_ptr<MagicPolygon>& GetMagicPolygon()const { return m_magic; }
	// ゲームUI
	const std::weak_ptr<Game>& GetGame()const { return m_game; }
	// ゲーム終了UI
	const std::weak_ptr<Exit>& GetExit()const { return m_exit; }
	// カーソル
	const std::weak_ptr<Cursor>& GetCursor()const { return m_cursor; }
	

	// オブジェクトを動的に生成 ImGuiで使用====================================
	// タイトルカメラ
	void AddTitleCamera();
	// タイトルUI
	void AddTitle();
	// GameUI
	void AddGame();
	// ExitUI
	void AddExit();
	// セレクトUI
	void AddSelect();
	// 骨
	void AddBone();
	// 骨色違い
	void AddBoneAlpha();
	// ゴーレム
	void AddGolem();
	// 武器
	void AddWeapon(std::string _filePath, std::string _weaponName);
	// 地面
	void AddGround();
	// 魔法陣の台
	void AddCircle();
	// 壁
	void AddWall();
	// 空
	void AddSkyBox();
	// 武器切り替え
	void ChangeWeapon(std::string _swordName, std::string _shieldName);
	//============================================================================

	// スロー値
	const float& GetSlow()const { return m_slow; }
	// スローフラグ　trueならスロー中
	const bool GetSlowFlg()const { return m_slowFlg; }

private:

	// オブジェクトのポインタ==================================================
	// タイトルカメラ
	std::weak_ptr<TitleCamera>               m_titleCamera;
	// タイトルUI
	std::weak_ptr<Title>                     m_title;
	// GameUI
	std::weak_ptr<Game>                      m_game;
	// ExitUI
	std::weak_ptr<Exit>                      m_exit;
	// セレクトUI
	std::weak_ptr<Select>                    m_select;
	// カーソル
	std::weak_ptr<Cursor>                    m_cursor;
	// ゲームカメラ
	std::weak_ptr<GameCamera>                m_camera;
	// プレイヤー
	std::weak_ptr<Player>                    m_player;
	// 地面
	std::weak_ptr<Ground>                    m_ground;
	// 魔法陣の台
	std::weak_ptr<Circle>                    m_circle;
	// 魔法陣
	std::weak_ptr<MagicPolygon>              m_magic;
	// 壁
	std::weak_ptr<Wall>                      m_wall;
	// 空
	std::weak_ptr<SkyBox>                    m_skybox;
	// プレイヤーHP
	std::weak_ptr<Player_HP>                 m_PlayerHP;
	// プレイヤースタミナ
	std::weak_ptr<Player_Stamina>            m_PlayerStamina;
	// 階層UI
	std::weak_ptr<Floor>                     m_floor;
	// ゲーム状態UI
	std::weak_ptr<GameStateUI>               m_gameStateUI;
	// ゴーレム
	std::weak_ptr<Golem>                     m_golem;
	// 骨色違い用弾リスト
	std::vector<std::weak_ptr<BoneAlpha>>    m_BoneAlphaList;
	
	enum EnemyHPType
	{
		Normal,
		Boss,
		Num
	};

	// 敵HPリスト
	std::vector<std::weak_ptr<Enemy_HP>>     m_EnemyHPList[EnemyHPType::Num];
	//=========================================================================

	// オブジェクトのID
	UINT                                     m_id = 1;
	
	// スロー値
	float                                    m_slow = 1.0f;
	// スローフラグ
	bool                                     m_slowFlg = false;

	// 武器の名前配列 ImGuiで生成するときに使用
	std::vector<std::string>                 m_swordNameList;
	std::vector<std::string>                 m_shieldNameList;

	// 今のシーン
	std::string                              m_nowScene;

};
