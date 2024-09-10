#pragma once
#include"../../ActionBase.h"
#include"../Player_ActionState.h"

class Player_Run :public Player_ActionState
{
public:			    
	Player_Run()             { Init(); }
	~Player_Run()   override {};
				    
	void Start()    override;
	void Center()   override;
	void End()      override;

	void Event()override;
	void CameraTransform(Math::Vector3& _dir);

	void ChangeAction()override;

private:

};