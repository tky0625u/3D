#pragma once

// UI基底
#include"../../UIBase.h"

class GameStateUI :public UIBase,public std::enable_shared_from_this<GameStateUI>
{
public:
	GameStateUI() {};
	~GameStateUI()override {};

	void Update()override;

	// セッター==========================================================================
	void SetChangeAlpha(float _change); // アルファ値変化量
	void SetState(bool _IsClear);       // ステート
	//===================================================================================

	// ゲッター==========================================================================
	const float& GetAlphaChange()const; // アルファ値変化量
	//===================================================================================

private:
	float m_alphaChange = 0.05f; // アルファ値変化量

private:

	// ステート基底
	class StateBase
	{
	public:
		StateBase()  {};
		virtual ~StateBase() {};
		
		// ステート更新
		void StateUpdate(std::shared_ptr<GameStateUI> owner);

		virtual void Enter (std::shared_ptr<GameStateUI> owner) {};
		virtual void Update(std::shared_ptr<GameStateUI> owner) {};
		virtual void Exit  (std::shared_ptr<GameStateUI> owner) {};

	protected:
		float m_alpha = 0.0f; // アルファ値
	};

	// ゲームオーバー
	class GameOver :public StateBase
	{
	public:
		GameOver() {};
		~GameOver()override {};

		void Enter (std::shared_ptr<GameStateUI> owner)override;
		void Update(std::shared_ptr<GameStateUI> owner)override;
		void Exit  (std::shared_ptr<GameStateUI> owner)override;
	};

	// クリア
	class Clear :public StateBase
	{
	public:
		Clear() {};
		~Clear()override {};

		void Enter (std::shared_ptr<GameStateUI> owner)override;
		void Update(std::shared_ptr<GameStateUI> owner)override;
		void Exit  (std::shared_ptr<GameStateUI> owner)override;
	};

	std::shared_ptr<StateBase> m_state     = nullptr; // ステート
	std::shared_ptr<StateBase> m_NextState = nullptr; // 次のステート

};