#pragma once

class ObjectManager; // オブジェクトマネジャ
class GameCamera;    // ゲームカメラ
class Sword;         // 剣

class CharacterBase :public KdGameObject
{
public:

	// パラメータ
	 struct Param
	{
		unsigned int   Hp  = 1; // 体力
		unsigned int   Atk = 1; // 攻撃力
		         float Sp  = 1; // 素早さ
		unsigned int   Sm  = 1; // スタミナ

		 float JumpPow  = 0.0f; // ジャンプ力
		 float AtkRange = 0.0f; // 攻撃範囲
	};

	CharacterBase()                                   { Init(); }
	~CharacterBase()                         override {};

	void PreUpdate()                         override;
	void Update()                            override;
	void PostUpdate()                        override;
	virtual void GenerateDepthMapFromLight() override;
	virtual void DrawLit()                   override;
	virtual void Init()                      override;

	virtual void Action() {};                                      // 行動
	virtual void CrushingAction();                                 // やられ演出
	virtual void GroundCheck();                                    // 地面当たり判定
	virtual void BumpCheck();                                      // 横方向当たり判定
	void Rotate(Math::Vector3 _moveDir, float _angChange = 10.0f); // キャラクター回転
	void CameraTransform(Math::Vector3& _dir);                     // カメラの向きによって方向修正
	const bool IsAnimCheck(const std::string animName) const { return m_anime._Equal(animName); } // していてほしいアニメーションと現在のアニメーションの確認

	// セッター========================================================================================================
	void SetCamera       (std::shared_ptr<GameCamera> a_camera)                   { m_camera = a_camera; }
	void SetModel        (std::shared_ptr<KdModelWork> _model)                    { m_model = _model; }
	void SetSword        (std::shared_ptr<Sword> _sword)                          { m_sword = _sword; }
	void SetAnime        (std::string animeName, bool animeFlg, float animeSpeed) 
	{
		m_anime = animeName;
		m_animeFlg = animeFlg;
		m_animeSpeed = animeSpeed;
	}
	void SetForward      (Math::Vector3 _forward)                                 { m_forward = _forward; }
	void SetDir          (Math::Vector3 _dir)                                     { m_dir = _dir; }
	void SetMove(Math::Vector3 dir, float _moveSpeed = 0)
	{
		m_dir = dir;

		if (_moveSpeed == 0.0f)
		{
			m_MoveSpeed = m_param.Sp;
		}
		else
		{
			m_MoveSpeed = _moveSpeed;
		}
	}
	void SetParam        (int _hp, int _atk, float _speed, int _stamina);
	void SetStamina      (int _stamina)                                           { m_param.Sm = _stamina; }
	void SetATK          (int _atk)                                               { m_param.Atk = _atk; }
	void SetInviAddTime  (int _inviAddTime)                                       { m_inviAddTime = _inviAddTime; }
	void SetInviTime     (int _inviTime)                                          { m_inviTime = _inviTime; }
	void SetHitSphereSize(float _size)                                            { m_HitSphereSize = _size; }
	void SetAtkRange     (float _atkRange)                                        { m_param.AtkRange = _atkRange; }
	void SetJumpPow      (float _jumpPow)                                         { m_param.JumpPow = _jumpPow; }
	//=================================================================================================================

	// ゲッター========================================================================================================
	const Param&                             GetParam()                              const { return m_param; }
	const std::weak_ptr<ObjectManager>&      GetObjManager()                         const { return m_ObjectManager; }
	const std::weak_ptr<GameCamera>&         GetCamera()                             const { return m_camera; }
	const std::weak_ptr<Sword>&              GetSword()                              const { return m_sword; }
	const std::unique_ptr<KdDebugWireFrame>& GetDebugWire()                          const { return m_pDebugWire; }
	const std::string&                       GetAnime()                              const { return m_anime; }
	const Math::Matrix                       GetSwordMat()                           const { return m_model->FindWorkNode("sword.R")->m_localTransform * m_model->FindWorkNode("hand.R")->m_worldTransform * (Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle.y))*(Math::Matrix::CreateTranslation(m_mWorld.Translation()))); }
	const Math::Matrix                       GetShieldMat()                          const { return m_model->FindWorkNode("shield.L")->m_localTransform * m_model->FindWorkNode("forearm.L")->m_worldTransform * (Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle.y)) * (Math::Matrix::CreateTranslation(m_mWorld.Translation()))); }
	const Math::Matrix                       GetHitModelMat()                        const { return m_model->FindWorkNode("spine")->m_worldTransform * (Math::Matrix::CreateTranslation(m_mWorld.Translation()));}
	const Math::Vector3&                     GetDir()                                const { return m_dir; }
	const Math::Vector3&                     GetForward()                            const { return m_forward; }
	const int&                               GetinviAddTime()                        const { return m_inviAddTime; }
	const int&                               GetinviTime()                           const { return m_inviTime; }
	const float&                             GetDissolve()                           const { return m_dissolve; }
	const bool                               GetIsAnimator()                         const { return m_animator->IsAnimationEnd(); }
	//=================================================================================================================
	


protected:
	// パラメータ
	Param                        m_param;
	
	// 3Dモデル
	std::shared_ptr<KdModelWork> m_model           = nullptr;
	
	// カメラ
	std::weak_ptr<GameCamera>    m_camera;
	
	// 剣
	std::weak_ptr<Sword>         m_sword;
	
	// 移動方向
	Math::Vector3                m_dir             = Math::Vector3::Zero; //方向
	
	// 前方方向
	Math::Vector3                m_forward         = Math::Vector3::Zero;
	
	// 無敵付与時間　※攻撃を与えたときにその相手の無敵時間に付与する
	int                          m_inviAddTime     = 0;

	// 無敵時間 ※無敵時間がある時は攻撃を食らわない
	int                          m_inviTime        = 0;

	// 重力
	float                        m_gravity         = 0.0f;
	
	// 重力加速度
	const float                  m_gravityPow      = 0.1f;
	
	// ディゾルブ
	float                        m_dissolve        = 0.0f;
	
	// 素早さ補正
	const float                  m_SpeedCorrection = 0.2f;
	
	// 移動量
	float                        m_MoveSpeed       = 0.0f;
	
	// 壁との当たり判定用のスフィアサイズ
	float                        m_HitSphereSize   = 2.0f;


	// アニメーション変数=========================================

	// アニメーション
	std::shared_ptr<KdAnimator>  m_animator        = nullptr;
	// 現在のアニメーション
	std::string                  m_anime           = "Idol";
	// 前回のアニメーション
	std::string                  m_beforeAnime     = m_anime;
	// アニメーション速度
	float                        m_animeSpeed      = 1.0f;
	// アニメーションループフラグ
	bool                         m_animeFlg        = true;

	//============================================================
};