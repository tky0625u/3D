#include "MagicPolygon.h"

// シーンマネジャ
#include"../../../Scene/SceneManager.h"
// ステージマネジャ
#include"../../StageManager.h"
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