#include "MagicPolygon.h"

// シーンマネジャ
#include"../../../Scene/SceneManager.h"
// ゲームカメラ
#include"../../Camera/GameCamera/GameCamera.h"
// 魔法陣の台
#include"../Circle/Circle.h"

void MagicPolygon::Update()
{
	if (!SceneManager::Instance().m_stop)
	{
		// 次の状態が決まっていたら
		if (m_NextState)
		{
			// 変更
			m_state = m_NextState;
			// 変更したらリセット
			m_NextState.reset();
		}
		// ステート更新
		m_state->SatateUpdate(shared_from_this());

		// 色
		m_color = { m_rgb,m_rgb ,m_rgb ,1 };
	}

	// 行列
	// 拡縮
	Math::Matrix _scale = Math::Matrix::CreateScale(m_size);
	// 回転
	Math::Matrix _rotX= Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(90));
	Math::Matrix _rotY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle.y));
	// 座標
	Math::Matrix _trans = Math::Matrix::CreateTranslation(m_pos);
	// 魔法陣位置
	Math::Matrix _PointTrans;
	if (m_circle.expired() == false)_PointTrans = Math::Matrix::CreateTranslation(m_circle.lock()->GetMagicPolygonPoint());
	// 行列合成
	m_mWorld = _scale * _rotX * _rotY * _trans * _PointTrans;
}

void MagicPolygon::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_spPolygon, m_mWorld,m_color);
}

void MagicPolygon::DrawBright()
{
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_spPolygon, m_mWorld, m_color);
}

void MagicPolygon::Init()
{
	// ポリゴン生成
	m_spPolygon = std::make_shared<KdSquarePolygon>();
	// ポリゴン読み込み
	m_spPolygon->SetMaterial("Asset/Textures/MagicPolygon/magic circle red.png");

	// RGB値
	m_rgb = 0.0f;
	// 色
	m_color = { m_rgb,m_rgb ,m_rgb ,1 };

	// 角度
	m_angle = Math::Vector3::Zero;

	// ステート
	std::shared_ptr<Normal> _normal = std::make_shared<Normal>();
	m_state = _normal;
}

// 状態切り替え====================================================================================
// 通常
void MagicPolygon::NormalChange()
{
	std::shared_ptr<Normal> _normal = std::make_shared<Normal>();
	m_NextState = _normal;
	m_flow = MagicPolygon::Flow::EnterType;
}

// 解放
void MagicPolygon::NextChange()
{
	std::shared_ptr<Next> _next = std::make_shared<Next>();
	m_NextState = _next;
	m_flow = MagicPolygon::Flow::EnterType;
}
//=================================================================================================

// セッター========================================================================================
// 魔法陣の台
void MagicPolygon::SetCircle(std::shared_ptr<Circle> _circle)
{
	m_circle = _circle;
}

// 行列
void MagicPolygon::SetMatrix(Math::Matrix _mat)
{
	m_mWorld = _mat;
}

// RGB変化量
void MagicPolygon::SetRGBChange(float _change)
{
	m_rgbChange = _change;
}

// 角度変化量
void MagicPolygon::SetChangeAngle(float _change)
{
	m_ChangeAngle = _change;
}

// テレポート可能フラグ
void MagicPolygon::SetTeleportFlg(bool _teleport)
{
	m_TeleportFlg = _teleport;
}
//=================================================================================================

// ゲッター========================================================================================
// RGB変化量
const float& MagicPolygon::GetRGBChange() const
{
	return m_rgbChange;
}

// 角度変化量
const float& MagicPolygon::GetChangeAngle() const
{
	return m_ChangeAngle;
}

// テレポート可能フラグ
const bool MagicPolygon::GetTeleport() const
{
	return m_TeleportFlg;
}
//=================================================================================================

// ステート更新
void MagicPolygon::StateBase::SatateUpdate(std::shared_ptr<MagicPolygon> owner)
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


// Normal==========================================================================================
void MagicPolygon::Normal::Enter(std::shared_ptr<MagicPolygon> owner)
{
	// テレポート可能フラグがONならOFFにする
	if (owner->m_TeleportFlg)owner->m_TeleportFlg = false;
	// 次のフローへ
	owner->m_flow = MagicPolygon::Flow::UpdateType;
}

void MagicPolygon::Normal::Update(std::shared_ptr<MagicPolygon> owner)
{
	// RGB値を0にする
	if (owner->m_rgb != 0.0f)
	{
		owner->m_rgb = 0.0f;
	}
}

void MagicPolygon::Normal::Exit(std::shared_ptr<MagicPolygon> owner)
{
}

//=================================================================================================


// Next============================================================================================
void MagicPolygon::Next::Enter(std::shared_ptr<MagicPolygon> owner)
{
	// エフェクト
	m_handle = KdEffekseerManager::GetInstance().Play("Circle/Circle.efkefc", owner->m_mWorld.Translation(), owner->m_size, 1.0f, false).lock()->GetHandle();
	// SE
	KdAudioManager::Instance().Play("Asset/Sound/Game/SE/Object/MagicPolygon/ステータス上昇魔法2.wav", 0.1f, false);
	// 次のフローへ
	owner->m_flow = MagicPolygon::Flow::UpdateType;
}

void MagicPolygon::Next::Update(std::shared_ptr<MagicPolygon> owner)
{
	// 角度変更
	owner->m_angle.y += owner->m_ChangeAngle;
	// 360度を越えたら0度に戻す
	if (owner->m_angle.y > 360.0f)owner->m_angle.y -= 360.0f;
	// RGBが1ではなかったら増加
	if (owner->m_rgb < 1.0f)      owner->m_rgb += owner->m_rgbChange;
	// エフェクトが終わっている&テレポート可能フラグがOFFならONにする
	if (!KdEffekseerManager::GetInstance().IsPlaying(m_handle) && !owner->m_TeleportFlg)owner->m_TeleportFlg = true;
}

void MagicPolygon::Next::Exit(std::shared_ptr<MagicPolygon> owner)
{
}

//=================================================================================================