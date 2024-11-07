#pragma once

class ObjectManager;
class Player;
class EnemyBase;
class EnemyManager;
class WeaponBase;
class UIBase;
class CameraBase;

class BaseScene
{
public :

	BaseScene() { Init(); }
	virtual ~BaseScene() {}

	void PreUpdate();
	virtual void Update();
	virtual void PostUpdate();

	void PreDraw();
	void Draw();
	void DrawSprite();
	void DrawDebug();

	void DeleteWeaponList();
	void DeleteObjectList();
	void DeleteEnemyUIList();

	// オブジェクトリストを取得
	const std::shared_ptr<Player>&                  GetPlayer()    const { return m_player; }
	const std::shared_ptr<CameraBase>&              GetCamera()    const { return m_camera; }
	const std::vector<std::shared_ptr<EnemyBase>>&  GetEnemyList();
	const std::list<std::shared_ptr<KdGameObject>>& GetObjList()   const { return m_ObjectList; }
	const std::list<std::shared_ptr<WeaponBase>>&   GetWeaponList()const { return m_WeaponList; }
	const std::list<std::shared_ptr<UIBase>>&       GetUIList()    const { return m_UIList; }
	const std::list<std::shared_ptr<UIBase>>&       GetPlayerUIList()    const { return m_PlayerUIList; }
	const std::list<std::shared_ptr<UIBase>>&       GetEnemyUIList()    const { return m_EnemyUIList; }
	const std::shared_ptr<EnemyManager>& GetEnemyManager()const { return m_EnemyManager; }

	const std::shared_ptr<ObjectManager> GetObjectManager() const { return m_ObjManager; }
	// オブジェクトリストに追加
	void SetPlayer(const std::shared_ptr<Player>& player)    { m_player = player; }
	void SetCamera(const std::shared_ptr<CameraBase>& camera){ m_camera = camera; }
	void AddObject(const std::shared_ptr<KdGameObject>& obj) { m_ObjectList.push_back(obj); }
	void AddEnemy(const std::shared_ptr<EnemyBase>& enemy);
	void AddWeapon(const std::shared_ptr<WeaponBase>& weapon){ m_WeaponList.push_back(weapon); }
	void AddUI    (const std::shared_ptr<UIBase>& ui)        { m_UIList.push_back(ui); }
	void AddPlayerUI    (const std::shared_ptr<UIBase>& ui)        { m_PlayerUIList.push_back(ui); }
	void AddEnemyUI    (const std::shared_ptr<UIBase>& ui)        { m_EnemyUIList.push_back(ui); }

	virtual void Init();

protected :

	// 継承先シーンで必要ならオーバーライドする
	virtual void Event();
	

	// 全オブジェクトのアドレスをリストで管理
	std::shared_ptr<Player>                  m_player = nullptr;
	std::shared_ptr<CameraBase>              m_camera = nullptr;
	std::shared_ptr<EnemyManager>            m_EnemyManager = nullptr;
	std::list<std::shared_ptr<WeaponBase>>   m_WeaponList;
	std::list<std::shared_ptr<KdGameObject>> m_ObjectList;
	std::list<std::shared_ptr<UIBase>>       m_UIList;
	std::list<std::shared_ptr<UIBase>>       m_PlayerUIList;
	std::list<std::shared_ptr<UIBase>>       m_EnemyUIList;

	std::shared_ptr<ObjectManager>           m_ObjManager = nullptr;
};
