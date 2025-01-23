#include "BulletBase.h"

void BulletBase::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}

void BulletBase::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}

void BulletBase::Init()
{
	m_ObjType = KdGameObject::ObjType::oObject;
}

// セッター==============================================================================
// 持ち主
void BulletBase::SetOwner(std::shared_ptr<CharacterBase> _owner)
{
	m_owner = _owner;
}

// 移動方向
void BulletBase::SetDir(Math::Vector3 _dir)
{
	m_dir = _dir;
}

// 攻撃力
void BulletBase::SetAtk(int _atk)
{
	m_atk = _atk;
}

// スピード
void BulletBase::SetSpeed(float _speed)
{
	m_speed = _speed;
}

// 破壊フラグ
void BulletBase::SetCrush(bool _crush)
{
	m_crush = _crush;
}

// モデル
void BulletBase::SetModel(std::shared_ptr<KdModelData> _model)
{
	m_model = _model;
}
//=======================================================================================

// ゲッター==============================================================================
// 方向
const Math::Vector3& BulletBase::GetDir() const
{
	return m_dir;
}

// 持ち主
const std::weak_ptr<CharacterBase>& BulletBase::GetOwner() const
{
	return m_owner;
}
//=======================================================================================