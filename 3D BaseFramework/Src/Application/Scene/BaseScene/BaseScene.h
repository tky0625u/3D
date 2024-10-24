#pragma once

class ObjectManager;

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

	// オブジェクトリストを取得
	const std::list<std::shared_ptr<KdGameObject>>& GetObjList()
	{
		return m_objList;
	}
	
	const std::shared_ptr<ObjectManager> GetObjectManager() const { return m_ObjManager; }
	// オブジェクトリストに追加
	void AddObject(const std::shared_ptr<KdGameObject>& obj)
	{
		m_objList.push_back(obj);
	}

	virtual void Init();

protected :

	// 継承先シーンで必要ならオーバーライドする
	virtual void Event();
	

	// 全オブジェクトのアドレスをリストで管理
	std::list<std::shared_ptr<KdGameObject>> m_objList;

	std::shared_ptr<ObjectManager>           m_ObjManager = nullptr;
};
