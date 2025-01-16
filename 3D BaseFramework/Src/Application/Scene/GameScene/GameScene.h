#pragma once

// シーン基底
#include"../BaseScene/BaseScene.h"

class GameScene : public BaseScene,public std::enable_shared_from_this<GameScene>
{
public:

	// ステートパターンフロー
	enum Flow
	{
		EnterType,
		UpdateType,
		ExitType
	};

	GameScene() {}
	~GameScene() {}

	void Init()      override;

	// デバッグウィンドウ
	void DebugObject()override;

private:

	void Event() override;

private:

	////////////////////////
	/// ステートパターン ///
	////////////////////////

	// 基底クラス
	class StateBase
	{
	public:
		StateBase()  {};
		virtual ~StateBase() {};

		void StateUpdate(std::shared_ptr<GameScene> owner)
		{
			switch (owner->m_flow)
			{
			case Flow::EnterType:
				Enter(owner);
				break;
			case Flow::UpdateType:
				Update(owner);
				break;
			case Flow::ExitType:
				Exit(owner);
				break;
			}
		}

		virtual void Enter (std::shared_ptr<GameScene> owner) {};
		virtual void Update(std::shared_ptr<GameScene> owner) {};
		virtual void Exit  (std::shared_ptr<GameScene> owner) {};

		// ステート切り替え
		virtual void ChangeState(std::shared_ptr<GameScene> owner) {};
	};

	// 通常
	class Normal :public StateBase
	{
	public:
		Normal ()         {};
		~Normal()override {};

		void Enter (std::shared_ptr<GameScene> owner)override;
		void Update(std::shared_ptr<GameScene> owner)override;
		void Exit  (std::shared_ptr<GameScene> owner)override;

		void ChangeState(std::shared_ptr<GameScene> owner)override;

	private:

	};

	// ゲームオーバー
	class GameOver :public StateBase
	{
	public:
		GameOver() {};
		~GameOver()override {};

		void Enter (std::shared_ptr<GameScene> owner)override;
		void Update(std::shared_ptr<GameScene> owner)override;

	private:

	};

	// クリア
	class Clear :public StateBase
	{
	public:
		Clear() {};
		~Clear()override {};

		void Enter (std::shared_ptr<GameScene> owner)override;
		void Update(std::shared_ptr<GameScene> owner)override;

	private:

	};

	// ステート
	std::shared_ptr<StateBase> m_state     = nullptr;
	// 次のステート
	std::shared_ptr<StateBase> m_NextState = nullptr;
	// フロー
	UINT                       m_flow      = Flow::UpdateType;
};
