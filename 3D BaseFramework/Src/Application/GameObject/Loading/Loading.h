#pragma once

class Loading :public KdGameObject,public std::enable_shared_from_this<Loading>
{
public:
	Loading() {};
	~Loading()override {};

	void Update()    override;
	void DrawSprite()override;
	void Init()      override;

	// ステート変更================================================================================
	void TitleChange(); // タイトル
	void GameChange();  // ゲーム
	//=============================================================================================

private:


private:

	class StateBase
	{
	public:
		StateBase() {};
		virtual ~StateBase() {};

		virtual void SetLoadParam() {};

		virtual void Init(std::shared_ptr<Loading> owner){}
		virtual void Update(std::shared_ptr<Loading> owner) {};
		virtual void DrawSprite(std::shared_ptr<Loading> owner) {};

	protected:
		// 構造体
		struct Load
		{
			// テクスチャ
			std::shared_ptr<KdTexture> m_tex = nullptr;
			// 行列
			Math::Matrix               m_Mat = Math::Matrix::Identity;
			// 座標
			Math::Vector2              m_pos = Math::Vector2::Zero;
			// 中心位置
			Math::Vector2              m_pivot = { 0.5f,0.5f };
			// 色
			Math::Color                m_color;
			// 切り取り範囲
			Math::Rectangle            m_rect;
			// アルファ値
			float                      m_alpha = 0.0f;
			// 角度
			float                      m_angle = 0.0f;
			// 大きさ
			float                      m_size = 1.0f;
		};
	};

	// タイトル
	class TitleLoad :public StateBase
	{
	public:
		TitleLoad() {};
		~TitleLoad()override {};

		void SetLoadParam()override;

		void Init(std::shared_ptr<Loading> owner)override;
		void Update(std::shared_ptr<Loading> owner)override;
		void DrawSprite(std::shared_ptr<Loading> owner)override;

	private:

	};

	// ゲーム
	class GameLoad :public StateBase
	{
	public:
		GameLoad() {};
		~GameLoad()override {};

		void SetLoadParam()override;

		void Init(std::shared_ptr<Loading> owner)override;
		void Update(std::shared_ptr<Loading> owner)override;
		void DrawSprite(std::shared_ptr<Loading> owner)override;

	private:
		// テクスチャtタイプ
		enum TextureType
		{
			Circle, // 魔法陣
			String, // 文字
			Num     // 数
		};

		// 構造体
		Load m_load[Num];

		// 魔法陣角度変化量
		float m_CircleChangeAngle = 0.05f;

		// 文字アルファ値変化量
		float m_StringChangeAlpha = 0.25f;
		// 文字アルファ値補正値
		float m_StringCorrectAlpha = 1.5f;
	};

	std::shared_ptr<StateBase> m_state = nullptr;
};