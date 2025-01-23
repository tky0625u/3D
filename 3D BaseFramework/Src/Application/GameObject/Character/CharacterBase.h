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
	const bool IsAnimCheck(const std::string animName) const;      // していてほしいアニメーションと現在のアニメーションの確認

	// セッター========================================================================================================
	void SetCamera       (std::shared_ptr<GameCamera> a_camera);           // カメラ
	void SetModel        (std::shared_ptr<KdModelWork> _model);			   // モデル
	void SetSword        (std::shared_ptr<Sword> _sword);				   // 剣
	void SetAnime(std::string animeName, bool animeFlg, float animeSpeed); // アニメーション
	void SetForward      (Math::Vector3 _forward);						   // 前方方向
	void SetDir          (Math::Vector3 _dir);							   // 方向
	void SetMove(Math::Vector3 dir, float _moveSpeed = 0);				   // 移動
	void SetParam        (int _hp, int _atk, float _speed, int _stamina);  // パラメータ
	void SetStamina      (int _stamina);								   // スタミナ
	void SetATK          (int _atk);									   // 攻撃力
	void SetInviAddTime  (int _inviAddTime);							   // 無敵付与時間
	void SetInviTime     (int _inviTime);								   // 無敵時間
	void SetHitSphereSize(float _size);									   // 当たり判定用スフィアサイズ
	void SetAtkRange     (float _atkRange);								   // 攻撃範囲
	void SetJumpPow      (float _jumpPow);								   // ジャンプ力
	//=================================================================================================================

	// ゲッター========================================================================================================
	const Param&                             GetParam()       const; // パラメータ
	const std::weak_ptr<ObjectManager>&      GetObjManager()  const; // オブジェクトマネジャ
	const std::weak_ptr<GameCamera>&         GetCamera()      const; // カメラ
	const std::weak_ptr<Sword>&              GetSword()       const; // 剣
	const std::unique_ptr<KdDebugWireFrame>& GetDebugWire()   const; // デバッグワイヤー
	const std::string&                       GetAnime()       const; // アニメーション
	const Math::Matrix                       GetSwordMat()    const; // 剣位置ノード
	const Math::Matrix                       GetShieldMat()   const; // 盾位置ノード
	const Math::Matrix                       GetHitModelMat() const; // 当たり判定用スフィア位置ノード
	const Math::Vector3&                     GetDir()         const; // 方向
	const Math::Vector3&                     GetForward()     const; // 前方方向
	const int&                               GetinviAddTime() const; // 無敵付与時間
	const int&                               GetinviTime()    const; // 無敵時間
	const float&                             GetDissolve()    const; // ディゾルブ
	const bool                               GetIsAnimator()  const; // アニメーション終了フラグ
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