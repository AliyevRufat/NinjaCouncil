#include "pch.h"
#include "MeleeEnemy.h"
#include "Avatar.h"
#include "utils.h"
#include <iostream>

MeleeEnemy::MeleeEnemy(const Point2f& pos)
	:Enemy{ pos }
{
	m_Health = 80;
	m_MaxHealth = 80;
	m_Radius = { 400 };
	m_Offset = { 20 };
	m_pSpritesTexture = { new Texture{"Resources/Images/MeleeEnemy.png"} };
}

void MeleeEnemy::Draw() const
{
	Rectf srcRect, dstRect;
	int nrRows = 5;
	int currentColumn = m_AnimWaitMoveFrame % m_NrOfFrames;
	dstRect = m_Shape;
	srcRect.height = m_pSpritesTexture->GetHeight() / nrRows;
	srcRect.width = (m_pSpritesTexture->GetWidth() - m_FrameOffset) / float(m_NrOfFrames);
	srcRect.bottom = -srcRect.height * m_CurrentRow;
	if (m_ActionState == ActionState::attacking)
	{
		currentColumn = m_AnimAttackFrame % m_NrOfFrames;
	}
	else if (m_ActionState == ActionState::dead)
	{
		currentColumn = m_AnimDeathFrame % m_NrOfFrames;
	}
	else if (m_ActionState == ActionState::takingdamage)
	{
		currentColumn = m_AnimDamageFrame % m_NrOfFrames;
	}
	srcRect.left = srcRect.width * currentColumn;
	dstRect.width = srcRect.width;
	dstRect.height = srcRect.height;
	Character::Draw(dstRect, srcRect);
}

void MeleeEnemy::Update(float elapsedSec, const Level& level, Avatar* pAvatar, HUD& hud)
{
	if (IsDead() && !m_DoOnce)
	{
		hud.IncreaseScore(30);
		m_DoOnce = true;
	}
	//the update function of this enemy is too different than its base class (Enemy) so I rewrite everything
	ElapsedSecCount(elapsedSec);
	ActionStates();

	level.HandleCollision(m_Shape, m_Velocity);
	if (m_ActionState != ActionState::dead && m_ActionState != ActionState::throwing)
	{
		if (m_ActionState != ActionState::attacking && m_ActionState != ActionState::takingdamage)
		{
			Behaviour(pAvatar);

			//Is waiting or moving
			if (level.IsOnGround(m_Shape, m_Velocity) && !m_Moving)
			{
				m_ActionState = ActionState::waiting;
			}
			else if (m_Moving)
			{
				m_ActionState = ActionState::moving;
			}
			//xand y axis move
			m_Shape.left += m_Velocity.x * elapsedSec / 5.f * 4.5f;
			m_Shape.bottom += m_Velocity.y * elapsedSec;
			m_Velocity += m_Acceleration * elapsedSec;
		}
	}
	//
	if (m_IsHit)
	{
		IsTakingDamage(pAvatar);
	}
	//
	Attack(pAvatar);
	//
	CollisionChecks(elapsedSec, pAvatar);
	//
	m_Delay += elapsedSec;
	//
}

void MeleeEnemy::Behaviour(const Avatar* pAvatar)
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
	//when to move
	//Checks for left side
	const int followRange = 140;
	if (pAvatar->GetShape().left - m_Shape.left > -m_Radius && pAvatar->GetShape().left - m_Shape.left < -m_Offset && pAvatar->GetShape().bottom - m_Shape.bottom <= followRange && pAvatar->GetShape().bottom - m_Shape.bottom >= -followRange / 2.f && !pAvatar->GetIsHit())
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
	//Checks for right side
	else if (pAvatar->GetShape().left - m_Shape.left < m_Radius && pAvatar->GetShape().left - m_Shape.left >m_Offset&& pAvatar->GetShape().bottom - m_Shape.bottom <= followRange && pAvatar->GetShape().bottom - m_Shape.bottom >= -followRange / 2.f && !pAvatar->GetIsHit())
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
	else
	{
		int slideCoef{ 15 };
		if (m_Velocity.x > (m_HorSpeed / slideCoef))
		{
			m_Velocity.x -= m_HorSpeed / slideCoef;
		}
		else if (m_Velocity.x < (-m_HorSpeed / slideCoef))
		{
			m_Velocity.x += m_HorSpeed / slideCoef;
		}
		else
		{
			m_Velocity.x = 0;
		}
		m_Velocity.x = 0;
		m_Moving = false;
		m_ActionState = ActionState::waiting;
	}
}

void MeleeEnemy::ActionStates()
{
	switch (m_ActionState)
	{
	case ActionState::attacking:
		m_FrameOffset = 60;
		m_NrOfFrames = 4;
		m_CurrentRow = 4;
		break;
	case ActionState::moving:
		m_FrameOffset = 0;
		m_NrOfFrames = 6;
		m_CurrentRow = 0;
		break;
	case ActionState::waiting:
		m_FrameOffset = 172;
		m_NrOfFrames = 4;
		m_CurrentRow = 1;
		break;
	case ActionState::dead:
		m_FrameOffset = 116;
		m_NrOfFrames = 4;
		m_CurrentRow = 2;
		break;
	case ActionState::takingdamage:
		m_FrameOffset = 236;
		m_NrOfFrames = 2;
		m_CurrentRow = 3;
		break;
	}
}

void MeleeEnemy::ElapsedSecCount(float elapsedSec)
{
	Character::ElapsedSecCount(elapsedSec);
	//if attacking
	if (m_ActionState == ActionState::attacking)
	{
		m_AnimWaitMoveFrame = 0;
		m_AnimThrowFrame = 0;
		m_AnimDamageFrame = 0;
		m_AnimTime += elapsedSec;
		if (m_AnimTime >= m_FrameChangeSpeed)
		{
			m_AnimAttackFrame++;

			if (m_AnimAttackFrame == m_NrOfFrames)
			{
				m_ActionState = ActionState::waiting;
			}
			m_AnimTime = 0;
		}
	}
}

void MeleeEnemy::Attack(Avatar* pAvatar)
{
	const int maxTime = 2;
	if (m_Delay > maxTime&& IsEnemyAttackingAvatar(pAvatar->GetShape()) && !pAvatar->IsDead() && !IsDead())
	{
		Sound::GetInstance()->PlaySoundEffect(12);
		m_ActionState = ActionState::attacking;
		pAvatar->OverlappingEnemyThrowableOrMelee(m_IsLookingLeft);
		m_Delay = 0;
	}
}

bool MeleeEnemy::IsEnemyAttackingAvatar(const Rectf& rect) const
{
	if (utils::IsOverlapping(m_Shape, rect))
	{
		return true;
	}
	else
	{
		return false;
	}
}