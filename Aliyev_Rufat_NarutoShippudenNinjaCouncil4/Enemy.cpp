#include "pch.h"
#include "Enemy.h"
#include <iostream>
#include "utils.h"
#include "Avatar.h"
#include "ExplosiveProjectile.h"

Enemy::Enemy(const Point2f& pos)
	: m_Throwable{ INT_MAX }
	, m_IsOnGround{ false }
	, m_Moving{ false }
	, m_Delay{ 0 }
	, m_Radius{ 400 }
	, m_Offset{ 250 }
	, m_CanSpawnAPickUp{ false }
	, m_HasAttacked{ false }
	, m_GoBack{ false }
	, m_IsPushed{ false }
	, m_DoOnce{ false }

	, m_StartPos{ Point2f{pos.x ,pos.y} }
{
	m_HorSpeed = 220;
	m_Shape = { Rectf{pos.x,pos.y,36,70} };
}

void Enemy::Update(float elapsedSec, const Level& level, Avatar* pAvatar, HUD& hud)
{
	if (level.IsOnGround(m_Shape, m_Velocity))
	{
		m_IsOnGround = true;
	}
	else
	{
		m_IsOnGround = false;
	}
	// drop out of level
	if (m_Shape.bottom <= -m_Shape.height)
	{
		m_Health = 0;
	}
	//
	level.HandleCollision(m_Shape, m_Velocity);
	if (m_ActionState != ActionState::dead && m_ActionState != ActionState::throwing)
	{
		if (m_ActionState != ActionState::attacking && m_ActionState != ActionState::takingdamage)
		{
			Behaviour(pAvatar);

			//Is waiting or moving
			if (m_IsOnGround && !m_Moving)
			{
				m_ActionState = ActionState::waiting;
			}
			else if (m_Moving)
			{
				m_ActionState = ActionState::moving;
			}
			//xand y axis move
			m_Shape.left += m_Velocity.x * elapsedSec;
			m_Shape.bottom += m_Velocity.y * elapsedSec;
			m_Velocity += m_Acceleration * elapsedSec;
		}
	}
	//Update Throwable
	if (!pAvatar->IsDead() && m_ActionState != ActionState::attacking && m_IsOnGround)
	{
		ThrowingWeapon(pAvatar);
	}
	m_Throwable.Update(elapsedSec);
	m_Throwable.IsNotRange(m_Shape);
	//
	if (m_IsHit)
	{
		IsTakingDamage(pAvatar);
	}
	//
	CollisionChecks(elapsedSec, pAvatar);
	//counter to throw
	m_Delay += elapsedSec;
}

void Enemy::Behaviour(const Avatar* pAvatar)
{
	//which way to look
	if (pAvatar->GetShape().left - m_Shape.left > 0 || m_Velocity.x > 0)
	{
		m_IsLookingLeft = false;
	}
	else if (pAvatar->GetShape().left - m_Shape.left < 0 || m_Velocity.x < 0)
	{
		m_IsLookingLeft = true;
	}
	//Checks for left side when to move
	const int followRangeX = 140;
	const int followRangeY = 70;
	if (pAvatar->GetShape().left - m_Shape.left > -m_Radius && pAvatar->GetShape().left - m_Shape.left < -m_Radius + m_Offset && pAvatar->GetShape().bottom - m_Shape.bottom <= followRangeX && pAvatar->GetShape().bottom - m_Shape.bottom >= -followRangeY && !pAvatar->GetIsHit())
	{
		if (m_Shape.left - m_StartPos.x >= -m_Radius && !m_GoBack)
		{
			m_Velocity.x = -m_HorSpeed;
			m_ActionState = ActionState::moving;
			m_Moving = true;
		}
		else if (m_Shape.left - m_StartPos.x <= -m_Radius)
		{
			m_GoBack = true;
		}
		//going back to starting position after the avatar moved a certain distance
		if (m_GoBack)
		{
			m_Velocity.x = m_HorSpeed;
			m_ActionState = ActionState::moving;
			m_Moving = true;
			m_IsLookingLeft = false;
			if (m_Shape.left >= m_StartPos.x)
			{
				m_GoBack = false;
			}
		}
	}
	//Checks for right side  when to move
	else if (pAvatar->GetShape().left - m_Shape.left < m_Radius && pAvatar->GetShape().left - m_Shape.left > m_Radius - m_Offset && pAvatar->GetShape().bottom - m_Shape.bottom <= followRangeX && pAvatar->GetShape().bottom - m_Shape.bottom >= -followRangeY && !pAvatar->GetIsHit())
	{
		if (m_Shape.left - m_StartPos.x <= m_Radius && !m_GoBack)
		{
			m_Velocity.x = m_HorSpeed;
			m_ActionState = ActionState::moving;
			m_Moving = true;
		}
		else if (m_Shape.left - m_StartPos.x >= m_Radius)
		{
			m_GoBack = true;
		}
		//going back to starting position after the avatar moved a certain distance
		if (m_GoBack)
		{
			m_Velocity.x = -m_HorSpeed;
			m_ActionState = ActionState::moving;
			m_Moving = true;
			m_IsLookingLeft = true;
			if (m_Shape.left <= m_StartPos.x)
			{
				m_GoBack = false;
			}
		}
	}
	else //stopping
	{
		m_Velocity.x = 0;
		m_Moving = false;
		m_ActionState = ActionState::waiting;
	}
}

void Enemy::ThrowingWeapon(const Avatar* pAvatar)
{
	const float halfWidthAvatar = pAvatar->GetShape().width / 2.f;
	const int maxTime = 2;
	if (m_Delay > maxTime&& m_ActionState == ActionState::waiting && !m_Throwable.GetIsThrowing() && abs(pAvatar->GetShape().left - m_Shape.left) <= m_Offset && abs(m_Shape.bottom - pAvatar->GetShape().bottom) <= halfWidthAvatar && m_ActionState != ActionState::takingdamage)
	{
		Sound::GetInstance()->PlaySoundEffect(13);
		m_ActionState = ActionState::throwing;
		m_Throwable.DecreaseThrowables();
		m_Throwable.IsLookingLeft(m_IsLookingLeft);
		m_Throwable.SetPosition(Point2f{ m_Shape.left, m_Shape.bottom });
		m_Throwable.SetIsThrowing(true);
		m_IsThrowing = true;
		m_Delay = 0;
	}
}

bool Enemy::IsAvatarOverlappingThrowable(const Rectf& rect)
{
	if (m_Throwable.IsOverlapping(rect))
	{
		m_Throwable.SetIsThrowing(false);
		return true;
	}
	else
	{
		return false;
	}
}

bool Enemy::GetCanSpawnAPickUp() const
{
	return m_CanSpawnAPickUp;
}

void Enemy::SetCanSpawnAPickUp(bool canSpawn)
{
	m_CanSpawnAPickUp = canSpawn;
}

void Enemy::IsTakingDamage(Avatar* pAvatar)
{
	if (m_IsPushed)
	{
		float pushSpeed = 70.0f;
		if (m_IsLookingLeft)
		{
			m_Velocity.x = -pushSpeed;
		}
		else
		{
			m_Velocity.x = pushSpeed;
		}
	}
	const int damage = 20;
	m_Health -= damage;
	m_ActionState = ActionState::takingdamage;
	m_Velocity.y = 0;
	m_IsAttacking = false;
	m_IsThrowing = false;
	m_IsHit = false;
	m_HasAttacked = false;
}

void Enemy::CollisionChecks(float elapsedSec, Avatar* pAvatar)
{
	const float avatarBiggerHitBox = pAvatar->GetShape().width * 1.5f;

	if (!IsDead())
	{
		if (pAvatar->IsEnemyOverlappingSpecialAttack(m_Shape))
		{
			const int pushBackSpeed = 500;
			const int startingFrameToPush = 15;
			const int endingFrameToPush = 21;
			const int maxFrames = 40;
			m_ActionState = ActionState::takingdamage;
			if (pAvatar->GetSpecialAttackFrame() >= startingFrameToPush && pAvatar->GetSpecialAttackFrame() <= endingFrameToPush)
			{
				if (m_IsLookingLeft)
				{
					m_Shape.left += pushBackSpeed * elapsedSec;
				}
				else
				{
					m_Shape.left -= pushBackSpeed * elapsedSec;
				}
			}
			else if (pAvatar->GetSpecialAttackFrame() == maxFrames - 1)
			{
				m_Shape.left += pushBackSpeed * 2 * elapsedSec;
				m_Health -= m_MaxHealth;
			}
		}
		else if (pAvatar->IsEnemyOverlappingThrowable(m_Shape))
		{
			m_IsHit = true;
		}
		else if (pAvatar->GetIsAttacking() && utils::IsOverlapping(m_Shape, Rectf{ pAvatar->GetShape().left , pAvatar->GetShape().bottom, avatarBiggerHitBox, pAvatar->GetShape().height }))
		{
			m_IsHit = true;
			m_IsPushed = true;
			pAvatar->SetIsAttacking(false);
		}
	}
	if (IsAvatarOverlappingThrowable(pAvatar->GetShape()))
	{
		m_IsThrowing = false;
		pAvatar->OverlappingEnemyThrowableOrMelee(m_IsLookingLeft);
	}
}

bool Enemy::IsDead()
{
	if (m_Health <= 0)
	{
		m_ActionState = ActionState::dead;
		m_Health = 0;
		return true;
	}
	return false;
}