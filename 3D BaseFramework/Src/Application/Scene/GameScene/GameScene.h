#pragma once

#include"../BaseScene/BaseScene.h"

class GameScene : public BaseScene,public std::enable_shared_from_this<GameScene>
{
public:

	enum Flow
	{
		EnterType,
		UpdateType,
		ExitType
	};

	GameScene() {}
	~GameScene() {}

	void Update()    override;
	void PostUpdate()override;
	void Init()      override;

	void DebugObject()override;
private:

	void Event() override;

private:

	class StateBase
	{
	public:
		StateBase()  {};
		virtual ~StateBase() {};

		virtual void Enter (std::shared_ptr<GameScene> owner) {};
		virtual void Update(std::shared_ptr<GameScene> owner) {};
		virtual void Exit  (std::shared_ptr<GameScene> owner) {};

		virtual void ChangeState(std::shared_ptr<GameScene> owner) = 0;
	};

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

	class GameOver :public StateBase
	{
	public:
		GameOver() {};
		~GameOver()override {};

		void Enter (std::shared_ptr<GameScene> owner)override;
		void Update(std::shared_ptr<GameScene> owner)override;
		void Exit  (std::shared_ptr<GameScene> owner)override;

		void ChangeState(std::shared_ptr<GameScene> owner)override;

	private:

	};

	class Clear :public StateBase
	{
	public:
		Clear() {};
		~Clear()override {};

		void Enter (std::shared_ptr<GameScene> owner)override;
		void Update(std::shared_ptr<GameScene> owner)override;
		void Exit  (std::shared_ptr<GameScene> owner)override;

		void ChangeState(std::shared_ptr<GameScene> owner)override;

	private:

	};

	std::shared_ptr<StateBase> m_state     = nullptr;
	std::shared_ptr<StateBase> m_NextState = nullptr;
	UINT                       m_flow      = Flow::UpdateType;
};
