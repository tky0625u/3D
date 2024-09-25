#include "ActionBase.h"
#include"../CharacterBase.h"

void ActionBase::Rotate(Math::Vector3 _moveDir, std::shared_ptr<CharacterBase> _target, float _angChange)
{

	//今の方向
	Math::Matrix  nowRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(_target->GetParam().Angle));
	Math::Vector3 nowVec = Math::Vector3::TransformNormal(Math::Vector3(_target->GetParam().ForwardX, _target->GetParam().ForwardY, _target->GetParam().ForwardZ), nowRot);

	//向きたい方向
	Math::Vector3 toVec = _moveDir;
	toVec.Normalize();

	//内角 回転する角を求める
	float d = nowVec.Dot(toVec);
	d = std::clamp(d, -1.0f, 1.0f); //誤差修正

	//回転角度を求める
	float ang = DirectX::XMConvertToDegrees(acos(d));

	//角度変更
	if (ang >= 0.1f)
	{
		if (ang > _angChange)
		{
			ang = _angChange; //変更角度
		}

		//外角　どっち回転かを求める
		Math::Vector3 c = toVec.Cross(nowVec);
		if (c.y >= 0)
		{
			//右回転
			_target->GetParam().Angle -= ang;
			if (_target->GetParam().Angle < 0.0f)
			{
				_target->GetParam().Angle += 360.0f;
			}
		}
		else
		{
			//左回転
			_target->GetParam().Angle += ang;
			if (_target->GetParam().Angle >= 360.0f)
			{
				_target->GetParam().Angle -= 360.0f;
			}
		}
	}
}
