﻿#pragma once

class CharacterBase;
class ObjectManager;

// ゲーム上に存在するすべてのオブジェクトの基底となるクラス
class KdGameObject //: public std::enable_shared_from_this<KdGameObject>
{
public:

	// どのような描画を行うのかを設定するTypeID：Bitフラグで複数指定可能
	enum
	{
		eDrawTypeLit = 1 << 0,
		eDrawTypeUnLit = 1 << 1,
		eDrawTypeBright = 1 << 2,
		eDrawTypeUI = 1 << 3,
		eDrawTypeDepthOfShadow = 1 << 4,
	};

	// オブジェクトタイプ　そのオブジェクトがどのタイプになるのかを設定
	enum ObjType
	{
		oPlayer,
		oEnemy,
		oObject,
	};

	// 現在のステートの実行関数タイプ
	enum Flow
	{
		EnterType,
		UpdateType,
		ExitType
	};

	KdGameObject() {}
	virtual ~KdGameObject() { Release(); }

	// 生成される全てに共通するパラメータに対する初期化のみ
	virtual void Init() {}

	virtual void PreUpdate() {}
	virtual void Update() {}
	virtual void PostUpdate() {}

	// それぞれの状況で描画する関数
	virtual void GenerateDepthMapFromLight() {}
	virtual void PreDraw() {}
	virtual void DrawLit() {}
	virtual void DrawUnLit() {}
	virtual void DrawBright() {}
	virtual void DrawSprite() {}

	virtual void DrawDebug();

	virtual void SetAsset(const std::string&) {}

	//virtual void SetPos(const Math::Vector3& pos) { m_mWorld.Translation(pos); }
	//virtual Math::Vector3 GetPos() const { return m_mWorld.Translation(); }
	virtual void SetPos(const Math::Vector3 pos) { m_pos = pos; }
	virtual Math::Vector3 GetPos() const { return m_pos; }

	// 拡大率を変更する関数
	void SetScale(float scalar);
	virtual void SetScale(const Math::Vector3& scale);
	virtual Math::Vector3 GetScale() const;

	void SetAngleY(float angleY);

	void SetID(UINT _id) { m_id = _id; }
	UINT GetID()const { return m_id; }

	void SetName(std::string _name) { m_name = _name; }
	const std::string GetName()const {return m_name; }

	void SetSize(float _size) { m_size = _size; }
	const float GetSize()const {return m_size; }

	void SetAngle(Math::Vector3 _angle) { m_angle = _angle; }
	const Math::Vector3 GetAngle()const {return m_angle; }

	void SetObjManager(std::shared_ptr<ObjectManager> _Obj) { m_ObjectManager = _Obj; }

	const Math::Matrix& GetMatrix() const { return m_mWorld; }

	void Expired() { m_isExpired = true; }// 消滅フラグ
	virtual bool IsExpired() const { return m_isExpired; }

	virtual bool IsVisible()	const { return false; }
	virtual bool IsRideable()	const { return false; }

	// 視錐台範囲内に入っているかどうか
	virtual bool CheckInScreen(const DirectX::BoundingFrustum&) const { return false; }

	// カメラからの距離を計算
	virtual void CalcDistSqrFromCamera(const Math::Vector3& camPos);

	float GetDistSqrFromCamera() const { return m_distSqrFromCamera; }

	UINT GetDrawType() const { return m_drawType; }

	UINT GetObjType()const { return m_ObjType; }

	bool Intersects(const KdCollider::SphereInfo& targetShape, std::list<KdCollider::CollisionResult>* pResults);
	bool Intersects(const KdCollider::BoxInfo& targetBox, std::list<KdCollider::CollisionResult>* pResults);
	bool Intersects(const KdCollider::RayInfo& targetShape, std::list<KdCollider::CollisionResult>* pResults);

protected:

	void Release() {}

	// 描画タイプ・何の描画を行うのかを決める / 最適な描画リスト作成用
	UINT m_drawType = 0;

	//オブジェクトタイプ
	UINT m_ObjType = oObject;

	// カメラからの距離
	float m_distSqrFromCamera = 0;

	// 存在消滅フラグ
	bool m_isExpired = false;

	// 3D空間に存在する機能
	Math::Matrix	m_mWorld;

	// 当たり判定クラス
	std::unique_ptr<KdCollider> m_pCollider = nullptr;

	// デバッグ情報クラス
	std::unique_ptr<KdDebugWireFrame> m_pDebugWire = nullptr;

	// オブジェクトの名前
	std::string m_name;

	Math::Vector3 m_pos = Math::Vector3::Zero;

	float m_size = 0.0f;

	Math::Vector3 m_angle = Math::Vector3::Zero;

	// ID
	UINT m_id = 0;

	// 現在のステートの実行関数タイプ
	UINT m_flow = Flow::UpdateType;

	// オブジェクト管理クラス
	std::weak_ptr<ObjectManager> m_ObjectManager;
};
