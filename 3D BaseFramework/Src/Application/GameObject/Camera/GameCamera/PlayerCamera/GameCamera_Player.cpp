#include "GameCamera_Player.h"
#include"../../../../Scene/SceneManager.h"
#include"../GameCamera.h"
#include"../../../Character/Action/Player/Player_ConText.h"
#include"../../../Character/Player/Player.h"
#include"../../../Character/Enemy/EnemyBase.h"

void PlayerCamera::Update()
{
}

void PlayerCamera::PostUpdate()
{
	// ターゲットの行列(有効な場合利用する)
	Math::Matrix								_targetMat = Math::Matrix::Identity;
	const std::shared_ptr<const KdGameObject>	_spTarget = m_target.lock()->GetwpTarget().lock();
	if (_spTarget)
	{
		_targetMat = Math::Matrix::CreateTranslation(_spTarget->GetPos());
	}

	if (!m_target.lock()->GetwpTarget().lock()->GetConText()->GetLockONFlg())
	{
		m_target.lock()->UpdateRotateByMouse();
	}
	else
	{
		LockON();
	}

	m_mRotation = m_target.lock()->GetRotationMatrix();
	m_mLocalPos = Math::Matrix::CreateTranslation(m_target.lock()->GetNowPos());

	m_mWorld = m_mLocalPos * m_mRotation * _targetMat;
	m_target.lock()->SetMatrix(m_mWorld);
}

void PlayerCamera::LockON()
{
	if (m_target.lock()->GetwpTarget().lock()->GetConText()->GetLockONTarget().expired())return;
	std::shared_ptr<EnemyBase> _target = m_target.lock()->GetwpTarget().lock()->GetConText()->GetLockONTarget().lock();

	// マウスでカメラを回転させる処理
	POINT _nowPos;
	GetCursorPos(&_nowPos);

	POINT _mouseMove{};
	_mouseMove.y = _nowPos.y - m_target.lock()->GetFixMousePos().y;

	Math::Vector3 _DegAng = m_target.lock()->GetNowDegAng();

	// 実際にカメラを回転させる処理(0.15はただの補正値)
	_DegAng.x += _mouseMove.y * 0.15f;

	// 回転制御
	_DegAng.x = std::clamp(_DegAng.x, -20.f, 45.f);

	Math::Vector3 _nowVec = Math::Vector3::TransformNormal(Math::Vector3{ 0,0,1 }, m_target.lock()->GetRotationYMatrix());

	Math::Vector3 _dir = _target->GetPos() - m_target.lock()->GetwpTarget().lock()->GetPos();
	_dir.y = 0.0f;
	_dir.Normalize();

	float d = _nowVec.Dot(_dir);
	d = std::clamp(d, -1.0f, 1.0f);

	float _ang = DirectX::XMConvertToDegrees(acos(d));

	//角度変更
	if (_ang >= 0.45f)
	{
		if (_ang > 10.0f)
		{
			_ang = 10.0f; //変更角度
		}

		//外角　どっち回転かを求める
		Math::Vector3 c = _dir.Cross(_nowVec);
		float angle = _DegAng.y;
		if (c.y >= 0)
		{
			//右回転
			angle -= _ang;
			if (angle < 0.0f)
			{
				angle += 360.0f;
			}

			_DegAng.y = angle;
		}
		else
		{
			//左回転
			angle += _ang;
			if (angle >= 360.0f)
			{
				angle -= 360.0f;
			}

			_DegAng.y = angle;
		}
	}

	m_target.lock()->SetDegAng(_DegAng);
}
