#include "EnemyManager.h"

// 敵基底
#include"EnemyBase.h"
// プレイヤー
#include"../Player/Player.h"

void EnemyManager::DeleteEnemyList()
{
	auto enemy = m_EnemyList.begin();

	while (enemy != m_EnemyList.end())
	{
		// 消滅フラグが立っていたら削除
		if ((*enemy)->IsExpired())
		{
			enemy = m_EnemyList.erase(enemy);
		}
		else
		{
			++enemy;
		}
	}
}

void EnemyManager::EnemyAttack()
{
	if (m_EnemyAttackList.size() == 0)return; // 攻撃リストが空だったらリターン

	for (auto& enemy : m_EnemyList)
	{
		// 攻撃中の敵の攻撃範囲にプレイヤーがいたらリターン　※同時攻撃防止
		if (enemy->GetActionType() == EnemyBase::Action::AttackType)
		{
			// プレイヤーとの距離
			float _dist = 0.0f;
			_dist = (enemy->GetTarget().lock()->GetPos() - enemy->GetPos()).Length();
			if (_dist <= enemy->GetParam().AtkRange)return;
		}
	}

	// 攻撃できる状態なら攻撃
	if (m_EnemyAttackList[0].expired() == false && // 消滅フラグがOFF
		m_EnemyAttackList[0].lock()->GetParam().Hp > 0 && // HPがある
		m_EnemyAttackList[0].lock()->GetActionType() == EnemyBase::Action::IdolType) // 待機状態
	{
		// 攻撃
		m_EnemyAttackList[0].lock()->AttackChange();
	}

	auto enemy = m_EnemyAttackList.begin();
	while (enemy != m_EnemyAttackList.end())
	{
		// 攻撃できない状態なら削除
		if (enemy == m_EnemyAttackList.begin() || // リストの先頭
			(*enemy).expired() || // 消滅フラグがON
			(*enemy).lock()->GetParam().Hp <= 0 || // HPがない
			(*enemy).lock()->GetActionType() != EnemyBase::Action::IdolType) // 待機状態以外
		{
			enemy = m_EnemyAttackList.erase(enemy);
		}
		else { ++enemy; }
	}
}

void EnemyManager::EnemyRun()
{
	for (auto& enemy : m_EnemyList)
	{
		// 移動状態でも待機状態でもなかったら飛ばす
		if (enemy->GetActionType() != EnemyBase::Action::RunType &&
			enemy->GetActionType() != EnemyBase::Action::IdolType )continue;

		// HPがなかったら飛ばす
		if (enemy->GetParam().Hp <= 0)continue;

		// 骨&骨色違い
		if (enemy->GetName() == "Bone" || enemy->GetName() == "BoneAlpha")
		{
			// プレイヤー判定
			// レイ判定
			KdCollider::RayInfo rayInfo;
			// 座標
			rayInfo.m_pos = enemy->GetPos();
			rayInfo.m_pos.y = (enemy->GetTarget().lock()->GetEnemyAttackPointMat().Translation().y); // プレイヤーの高さ
			// 方向
			Math::Matrix nowRotY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(enemy->GetAngle().y)); // 現在の回転行列
			Math::Vector3 nowVec = Math::Vector3::TransformNormal(Math::Vector3{ enemy->GetForward().x,enemy->GetForward().y,enemy->GetForward().z }, nowRotY); // 現在の方向
			nowVec.Normalize(); //正規化
			rayInfo.m_dir = nowVec;
			// 長さ
			rayInfo.m_range = enemy->GetParam().AtkRange; // 攻撃範囲
			// 対象
			rayInfo.m_type = KdCollider::Type::TypeBump;

			std::list<KdCollider::CollisionResult> _List; // 当たり判定リスト
			if (enemy->GetTarget().lock()->Intersects(rayInfo, &_List))
			{
				// 一度待機状態にする
				enemy->IdolChange();

				bool atkFlg = false;
				for (auto& atk : m_EnemyAttackList)
				{
					if (atk.expired())continue;
					if (atk.lock()->GetID() == enemy->GetID()) // すでに攻撃予定なら飛ばす
					{
						atkFlg = true;
						break;
					}
				}
				if (atkFlg)continue;

				// 攻撃予定に入ってなかったら追加
				m_EnemyAttackList.push_back(enemy);

				continue;
			}
			else if (enemy->GetActionType() != EnemyBase::Action::RunType)
			{
				// 攻撃範囲外なら移動
				enemy->RunChange();
			}
		}
		// ゴーレム
		else if (enemy->GetName() == "Golem")
		{
			// プレイヤー座標
			Math::Vector3 _playerPos = enemy->GetTarget().lock()->GetPos();
			_playerPos.y = 0.0f; // Y軸は考慮しない
			// ゴーレム座標
			Math::Vector3 _enemyPos = enemy->GetPos();
			_enemyPos.y = 0.0f; // Y軸は考慮しない
			// プレイヤー世の距離
			float _dist = (_playerPos - _enemyPos).Length();
			
			// プレイヤーが攻撃範囲内なら
			if (_dist <= enemy->GetParam().AtkRange)
			{
				// 一度待機状態にする
				enemy->IdolChange();

				bool atkFlg = false;
				for (auto& atk : m_EnemyAttackList)
				{
					if (atk.expired())continue;
					if (atk.lock()->GetID() == enemy->GetID()) // すで攻撃予定なら飛ばす
					{
						atkFlg = true;
						break;
					}
				}
				if (atkFlg)continue;

				// 攻撃予定に入ってなかったら追加
				m_EnemyAttackList.push_back(enemy);

				continue;
			}
			// 攻撃範囲外なら移動
			else if (enemy->GetActionType() != EnemyBase::Action::RunType)
			{
				enemy->RunChange();
			}
		}
	}
}

void EnemyManager::PreUpdate()
{
	for (auto& enemy : m_EnemyList) enemy->PreUpdate();
}

void EnemyManager::Update()
{
	EnemyRun();
	EnemyAttack();
	for (auto& enemy : m_EnemyList) enemy->Update();
}

void EnemyManager::PostUpdate()
{
	DeleteEnemyList();
	for (auto& enemy : m_EnemyList) enemy->PostUpdate();
}

void EnemyManager::GenerateDepthMapFromLight()
{
	for (auto& enemy : m_EnemyList) enemy->GenerateDepthMapFromLight();
}

void EnemyManager::PreDraw()
{
	for (auto& enemy : m_EnemyList) enemy->PreDraw();
}

void EnemyManager::DrawLit()
{
	for (auto& enemy : m_EnemyList)enemy->DrawLit();
}

void EnemyManager::DrawUnLit()
{
	for (auto& enemy : m_EnemyList) enemy->DrawUnLit();
}

void EnemyManager::DrawBright()
{
	for (auto& enemy : m_EnemyList) enemy->DrawBright();
}

void EnemyManager::DrawSprite()
{
	for (auto& enemy : m_EnemyList) enemy->DrawSprite();
}

void EnemyManager::DrawDebug()
{
	for (auto& enemy : m_EnemyList)enemy->DrawDebug();
}

void EnemyManager::Init()
{
	for (auto& enemy : m_EnemyList) enemy->Init();
}
