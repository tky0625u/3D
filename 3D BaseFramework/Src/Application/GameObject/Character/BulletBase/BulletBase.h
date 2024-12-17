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
	void SetCrush(bool _crush)                           { m_crush = _crush; } // 破壊フラグ
	void SetSpeed(float _speed)                          { m_speed = _speed; } // 移動量
	void SetModel(std::shared_ptr<KdModelData> _model)   { m_model = _model; } // モデル
	//=============================================================================================

	// ゲッター====================================================================================
	const Math::Vector3&                GetDir()  const { return m_dir; }
	const std::weak_ptr<CharacterBase>& GetOwner()const { return m_owner; }
	//=============================================================================================

protected:
	std::weak_ptr<CharacterBase> m_owner;
	std::shared_ptr<KdModelData> m_model = nullptr;
	Math::Vector3                m_dir   = Math::Vector3::Zero;
	float                        m_speed = 2.0f;
	bool                         m_crush = false;
};