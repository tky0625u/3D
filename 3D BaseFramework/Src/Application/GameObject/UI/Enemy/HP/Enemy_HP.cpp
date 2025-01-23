#include "Enemy_HP.h"

// ゲームカメラ
#include"../../../Camera/GameCamera/GameCamera.h"
// 敵基底
#include"../../../Character/Enemy/EnemyBase.h"

void Enemy_HP::Update()
{
	if (m_target.expired()) // ターゲットが消滅していたら
	{
		// 消滅
		if (!IsExpired())Expired();
		return;
	}

	if (m_rect[HP::hp].width > m_rect[HP::down].width) // 幅が減少ゲージよりもHPゲージの方が大きかったら
	{
		// HPゲージと同じ幅にする
		m_rect[HP::down].width = m_rect[HP::hp].width;
	}

	// HP減少
	if (m_target.lock()->GetParam().Hp != m_beforeHP) // 前回とHPが変わっていたら
	{//               最大幅     最大HP               現在のHP
		float hp = (m_MaxWidth / m_MaxHP) * m_target.lock()->GetParam().Hp; // 幅計算
		m_rect[HP::hp] = { 0,0,long(hp),64 }; // HPゲージ変更
		m_beforeHP = m_target.lock()->GetParam().Hp; // 前回のHPを更新
		m_NowDownTime = m_DownTime;
	}

	// 減少ゲージ減少
	if (m_NowDownTime > 0)m_NowDownTime--;
	else // 減少開始時間が0になったら
	{
		m_NowDownTime = 0;
		if (m_rect[HP::down].width != m_rect[HP::hp].width)m_rect[HP::down].width -= m_DownChange; // 現在のHPまで徐々に減少
	}

	// ステート更新
	m_state->Update(shared_from_this());
}

void Enemy_HP::DrawSprite()
{
	if (m_camera.lock()->GetCameraType() == GameCamera::CameraType::BossType)return;

	for (int h = 0; h < HP::num; ++h)
	{
		KdShaderManager::Instance().m_spriteShader.DrawTex(m_pTex[h], (int)(m_pos.x - (m_MaxWidth / 2)), (int)m_pos.y, (int)m_rect[h].width * m_size, (int)m_rect[h].height * m_size, &m_rect[h], &m_color, m_pivot);
	}
}

void Enemy_HP::Init()
{
	m_pivot = { 0.0f,0.5f };
	m_MaxWidth = 1024;
	m_alpha = 1.0f;
	m_color = { 1,1,1,m_alpha };
	if (m_target.expired())return;
	m_MaxHP = m_target.lock()->GetParam().Hp;
	m_beforeHP = m_MaxHP;

	//HPバー
	m_rect[HP::hp] = { 0,0,(long)m_MaxWidth,64 };
	m_pTex[HP::hp] = std::make_shared<KdTexture>();
	m_pTex[HP::hp]->Load("Asset/Textures/UI/Enemy/Enemy_HP_line.png");

	//HPボックス
	m_rect[HP::box] = { 0,0,(long)m_MaxWidth,64 };
	m_pTex[HP::box] = std::make_shared<KdTexture>();
	m_pTex[HP::box]->Load("Asset/Textures/UI/Enemy/frame.png");

	//HP減少ゲージ
	m_rect[HP::down] = { 0,0,(long)m_MaxWidth,64 };
	m_pTex[HP::down] = std::make_shared<KdTexture>();
	m_pTex[HP::down]->Load("Asset/Textures/UI/Enemy/Enemy_HP_Downline.png");

	if (m_target.lock()->GetBossFlg())
	{
		m_state = std::make_shared<Boss>();
		m_flow = Flow::UpdateType;
	}
	else
	{
		m_state = std::make_shared<Normal>();
		m_flow = Flow::UpdateType;
	}
}

// セッター========================================================================================
// ターゲット
void Enemy_HP::SetTarget(std::shared_ptr<EnemyBase> _target)
{
	m_target = _target;
}

// カメラ
void Enemy_HP::SetCamera(std::shared_ptr<GameCamera> _camera)
{
	m_camera = _camera;
}

// 減少ゲージ減少開始時間
void Enemy_HP::SetDownTime(int _downTime)
{
	m_DownTime = _downTime;
}

// 減少ゲージ変化量
void Enemy_HP::SetDownChange(float _downChange)
{
	m_DownChange = _downChange;
}

// X軸補正
void Enemy_HP::SetPosXCorrection(float _correction)
{
	m_posXCorrection = _correction;
}
//=================================================================================================

// ゲッター========================================================================================
// 減少ゲージ減少開始時間
const int& Enemy_HP::GetDownTime() const
{
	return m_DownTime; 
}

// 減少ゲージ変化量
const float& Enemy_HP::GetDownChange() const
{
	return m_DownChange;
}

// X軸補正
const float& Enemy_HP::GetPosXCorrection() const
{
	return m_posXCorrection;
}
//=================================================================================================

// Normal==============================================================================================================
void Enemy_HP::Normal::Update(std::shared_ptr<Enemy_HP> owner)
{
	if (owner->m_camera.expired())return; // カメラが無かったら早期リターン

	// 座標変換
	Math::Vector3 _pos = { owner->m_pos.x,owner->m_pos.y,0.0f }; // 現在の座標
	owner->m_camera.lock()->WorkCamera()->ConvertWorldToScreenDetail(owner->m_target.lock()->GetHPMat().Translation(), _pos); // 3D座標を2D座標に変換

	if (_pos.z >= 0.0f)owner->m_alpha = 1.0f; // Z軸が0以下=カメラの後ろだったら透明
	else { owner->m_alpha = -1.0f; }

	owner->m_pos = { _pos.x + owner->m_posXCorrection,_pos.y };
	owner->m_color = { 1,1,1,owner->m_alpha };
}
//=====================================================================================================================


// Boss================================================================================================================
void Enemy_HP::Boss::Update(std::shared_ptr<Enemy_HP> owner)
{

}
//=====================================================================================================================

