#pragma once

class CharacterBase;

class BulletBase :public KdGameObject,public std::enable_shared_from_this<BulletBase>
{
public:
	BulletBase()                                     {};
	~BulletBase()                           override {};

	virtual void Update()                   override {};
	virtual void PostUpdate()               override {};
	virtual void GenerateDepthMapFromLight()override;
	virtual void DrawLit()                  override;
	virtual void Init()                     override { m_ObjType = KdGameObject::ObjType::oObject; }

	// セッター====================================================================================
	void SetOwner(std::shared_ptr<CharacterBase> _owner) { m_owner = _owner; } // 発射したキャラクター
	void SetDir  (Math::Vector3 _dir)                    { m_dir = _dir; }     // 移動方向 
	void SetAtk  (int _atk)                              { m_atk = _atk; }     // 攻撃力
	void SetSpeed(float _speed)                          { m_speed = _speed; } // 移動量
	void SetCrush(bool _crush)                           { m_crush = _crush; } // 破壊フラグ
	void SetModel(std::shared_ptr<KdModelData> _model)   { m_model = _model; } // モデル
	//=============================================================================================

	// ゲッター====================================================================================
	const Math::Vector3&                GetDir()  const { return m_dir; }   // 方向
	const std::weak_ptr<CharacterBase>& GetOwner()const { return m_owner; } // 持ち主
	//=============================================================================================

protected:
	// 持ち主
	std::weak_ptr<CharacterBase> m_owner;
	// モデル
	std::shared_ptr<KdModelData> m_model = nullptr;
	// 方向
	Math::Vector3                m_dir   = Math::Vector3::Zero;
	// 攻撃力
	int                          m_atk   = 0;
	// 移動速度
	float                        m_speed = 2.0f;
	// 破壊フラグ
	bool                         m_crush = false;
};