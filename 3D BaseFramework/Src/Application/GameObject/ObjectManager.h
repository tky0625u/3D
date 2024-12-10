#pragma once

class Player;
class EnemyBase;
class Bone;
class GameCamera;
class Title;
class Game;
class Exit;
class Cursor;
class Select;
class TitleCamera;
class Ground;
class Circle;
class MagicPolygon;
class StageManager;
class Player_HP;
class Player_Stamina;
class Floor;
class Enemy_HP;
class GameStateUI;
class Golem;
class Bullet;
class BoneAlpha;

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
	void DeleteObjectList();
	
	// スロー状態の切替
	void SlowChange();

	// ワープ解放時の演出の準備
	void NextStageLiberation();

	// ゲームクリア
	void GameClear();

	// テレポート演出の準備
	void NextTeleport();

	// 次のステージを生成
	void CreateStage(std::shared_ptr<StageManager> _stage);


	// ImGuiで動的にオブジェクトの設定をする
	void DebugObject(std::shared_ptr<StageManager> _stage = nullptr);


	// Jsonファイルに保存======================================================
	void TitleCameraWrite();
	void TitleWrite();      
	void GameWrite();       
	void ExitWrite();       
	void SelectWrite();
	void CursorWrite();
	void GameCameraWrite();
	void PlayerWrite(std::string _fileName);
	void PlayerHPWrite();
	void PlayerStaminaWrite();
	void FloorWrite();
	void GameStateWrite();
	void EnemyWrite(int _stage, int _wave, std::string _fileName);
	void EnemyHPWrite();
	void SwordWrite(std::string _swordName, std::string _fileName);
	void ShieldWrite(std::string _shieldName, std::string _fileName);
	void ObjectWrite(std::string _fileName);
	//=========================================================================

	// モデルを事前に読み込む
	void ModelLoad();

	// オブジェクトの生成======================================================
	void SetTitleCamera();
	void SetTitleParam();
	void SetGameParam();
	void SetExitParam();
	void SetSelectParam();
	void SetCursorParam();
	void SetGameCameraParam(std::shared_ptr<StageManager> _stage);
	void SetObjectParam(std::shared_ptr<StageManager> _stage = nullptr);
	void SetPlayerParam(std::shared_ptr<StageManager> _stage);
	void SetPlayerUI(std::shared_ptr<StageManager> _stage);
	void SetPlayerHPParam();
	void SetPlayerStaminaParam();
	void SetFloorParam(std::shared_ptr<StageManager> _stage);
	void SetGameStateParam(bool _IsClear);
	void SetWeaponParam(std::string _filePath, std::string _weaponName);
	void SetEnemyParam(std::string _filePath = "none", std::shared_ptr<StageManager> _stage = nullptr);
	void SetEnemyHPParam(std::shared_ptr<EnemyBase> _enemy);
	std::shared_ptr<Bullet> SetBulletParam();
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
	void AddTitleCamera();
	void AddTitle();
	void AddGame();
	void AddExit();
	void AddSelect();
	void AddBone();
	void AddBoneAlpha();
	void AddGolem();
	void AddWeapon(std::string _filePath, std::string _weaponName);
	void AddGround();
	void AddCircle();
	void AddWall();
	void AddSkyBox();
	void ChangeWeapon(std::string _swordName, std::string _shieldName);
	//============================================================================

	// スロー値
	const float GetSlow()const { return m_slow; }
	// スローフラグ　trueならスロー中
	const bool GetSlowFlg()const { return m_slowFlg; }

private:

	// オブジェクトのポインタ==================================================
	std::weak_ptr<TitleCamera>               m_titleCamera;
	std::weak_ptr<Title>                     m_title;
	std::weak_ptr<Game>                      m_game;
	std::weak_ptr<Exit>                      m_exit;
	std::weak_ptr<Select>                    m_select;
	std::weak_ptr<Cursor>                    m_cursor;
	std::weak_ptr<GameCamera>                m_camera;
	std::weak_ptr<Player>                    m_player;
	std::weak_ptr<Ground>                    m_ground;
	std::weak_ptr<Circle>                    m_circle;
	std::weak_ptr<MagicPolygon>              m_magic;
	std::weak_ptr<Player_HP>                 m_PlayerHP;
	std::weak_ptr<Player_Stamina>            m_PlayerStamina;
	std::weak_ptr<Floor>                     m_floor;
	std::weak_ptr<GameStateUI>               m_gameStateUI;
	std::weak_ptr<Golem>                     m_golem;
	std::vector<std::weak_ptr<BoneAlpha>>    m_BoneAlphaList;
	std::vector<std::weak_ptr<Enemy_HP>>     m_EnemyHPList;
	//=========================================================================

	// オブジェクトのID
	UINT                                     m_id = 0;
	
	// スロー値
	float                                    m_slow = 1.0f;
	// スローフラグ
	bool                                     m_slowFlg = false;
	
	// 敵配列
	std::vector<std::weak_ptr<EnemyBase>>    m_EnemyList;
	// キャラクター以外の配列
	std::vector<std::weak_ptr<KdGameObject>> m_ObjectList;

	// 武器の名前配列 ImGuiで生成するときに使用
	std::vector<std::string>                 m_swordNameList;
	std::vector<std::string>                 m_shieldNameList;

	// 今のシーン
	std::string                              m_nowScene;

};
