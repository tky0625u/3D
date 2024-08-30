#include "Player.h"
#include"../../Camera/CameraBase.h"
#include"../Action/Player/Idol/Player_Idol.h"
#include"../Action/Player/Run/Player_Run.h"
#include"../Action/Player/Roll/Player_Roll.h"
#include"../Action/Player/Guard/Player_Guard.h"
#include"../Action/Player/Attack/Player_Attack.h"
#include"../Action/Player/Hit/Player_Hit.h"
#include"../Action/Player/GuardReaction/Player_GuardReaction.h"
#include"../Action/Player/Parry/Player_Parry.h"
#include"../Action/Player/Counter/Player_Counter.h"
#include"Application/main.h"
void Player::PreUpdate()
{
	//移動
	if (GetAsyncKeyState('W') & 0x8000 | GetAsyncKeyState('A') & 0x8000 | GetAsyncKeyState('S') & 0x8000 | GetAsyncKeyState('D') & 0x8000)
	{
		m_ActionType |= ActionType::Move;
	}
	else if(m_ActionType & ActionType::Move)
	{
		m_ActionType &= ~ActionType::Move;
	}

	//攻撃
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		m_ActionType |= ActionType::Attack;
	}
	else if (m_ActionType & ActionType::Attack)
	{
		m_ActionType &= ~ActionType::Attack;
	}

	//防御
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		m_ActionType |= ActionType::Guard;
	}
	else if (m_ActionType & ActionType::Guard)
	{
		m_ActionType &= ~ActionType::Guard;
	}

	//回避
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		m_ActionType |= ActionType::Roll;
	}
	else if (m_ActionType & ActionType::Roll)
	{
		m_ActionType &= ~ActionType::Roll;
	}


}

void Player::Action()
{
	m_dir = Math::Vector3::Zero; //ベクトルリセット
	float         Move = 0.0f;
	m_moveFlg = false;

	std::string BeforeAction = m_NowAction;
	m_action->KeyCheck(m_ActionType,m_BeforeActionType);
	if (m_NowAction != BeforeAction || m_NowAction=="Idol")
	{
		ChangeAction(m_NowAction);
	}

	int flow = m_action->GetFlow();

	if(flow == m_action->StartType){ m_action->Start(); }
	else if(flow == m_action->CenterType){ m_action->Center(); }
	else if(flow == m_action->EndType){ m_action->End(); }

	if (m_NowAction == "Guard" || m_NowAction == "GuardReaction")
	{
		m_guardFlg = true;
		m_ParryTime++;
	}
	else 
	{
		m_guardFlg = false;
		m_ParryTime = 0;
	}

	//回転
	if (m_moveFlg)
	{
		if (m_NowAction=="Run")
		{
			Math::Matrix cameraRotYMat = Math::Matrix::Identity;
			if (m_camera.expired() == false)
			{
				cameraRotYMat = m_camera.lock()->GetRotationYMatrix();
			}
			m_dir = m_dir.TransformNormal(m_dir, cameraRotYMat);

			m_dir.Normalize(); //正規化
		}

		CharacterBase::Rotation(m_dir);

		Move = m_param.Sp * m_SpeedCorrection;

		m_pos += Move * m_dir; //座標更新
	}

	m_BeforeActionType = m_ActionType;
}

void Player::Init()
{
	CharacterBase::Init();
	m_model->SetModelData("Asset/Models/Character/Player/Player.gltf");
	m_animator->SetAnimation(m_model->GetData()->GetAnimation(m_anime), m_animeFlg);

	ParamLoad("CSV/Character/Param/Player/Player.csv");

	std::shared_ptr<Player_Idol> idol = std::make_shared<Player_Idol>();
	idol->SetTarget(shared_from_this());
	RegisterAction("Idol", idol);

	std::shared_ptr<Player_Run> run = std::make_shared<Player_Run>();
	run->SetTarget(shared_from_this());
	RegisterAction("Run", run);

	std::shared_ptr<Player_Roll> roll = std::make_shared<Player_Roll>();
	roll->SetTarget(shared_from_this());
	RegisterAction("Roll", roll);

	std::shared_ptr<Player_Guard> guard = std::make_shared<Player_Guard>();
	guard->SetTarget(shared_from_this());
	RegisterAction("Guard", guard);

	std::shared_ptr<Player_Attack> attack = std::make_shared<Player_Attack>();
	attack->SetTarget(shared_from_this());
	RegisterAction("Attack", attack);

	std::shared_ptr<Player_Hit> hit = std::make_shared<Player_Hit>();
	hit->SetTarget(shared_from_this());
	RegisterAction("Hit", hit);

	std::shared_ptr<Player_GuardReaction> gr = std::make_shared<Player_GuardReaction>();
	gr->SetTarget(shared_from_this());
	RegisterAction("GuardReaction", gr);

	std::shared_ptr<Player_Parry> parry = std::make_shared<Player_Parry>();
	parry->SetTarget(shared_from_this());
	RegisterAction("Parrying", parry);

	std::shared_ptr<Player_Counter> counter = std::make_shared<Player_Counter>();
	counter->SetTarget(shared_from_this());
	RegisterAction("Counter", counter);

	m_action = idol;
	m_NowAction = "Idol";

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Player", m_model, KdCollider::TypeBump | KdCollider::TypeDamage | KdCollider::TypeEvent);

	m_ObjType = ObjType::oPlayer;
}

void Player::CrushingAction()
{
	if (m_anime != "Death")
	{
		m_anime = "Death";
		m_animeFlg = false;
		m_animeSpeed = 1.0f;
	}
}
