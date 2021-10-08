#include "pch.h"
#include "MeleeBoss.h"
#include "utils.h"
#include "Avatar.h"

MeleeBoss::MeleeBoss(const Point2f& pos)
	:Enemy{ pos }
	, m_NrOfSpecialAttackFrames{ 30 }
{
	m_pSpritesTexture = { new Texture{"Resources/Images/MeleeBoss.png"} };
}

void MeleeBoss::Update(float elapsedSec, const Level& level, Avatar* pAvatar, HUD& hud)
{
	if (IsDead() && !m_DoOnce)
	{
		hud.IncreaseScore(70);
		m_DoOnce = true;
	}
	//
	ElapsedSecCount(elapsedSec);
	ActionStates();
	Enemy::Update(elapsedSec, level, pAvatar, hud);
	SpecialAttack(elapsedSec);
	if (IsEnemyAttackingAvatar(pAvatar->GetShape()) && !m_HasAttacked)
	{
		pAvatar->OverlappingEnemySpecialAttackOrDeathExplosion(elapsedSec, m_IsLookingLeft);
		m_HasAttacked = true;
	}
}

void MeleeBoss::Draw() const
{
	const int nrRows = 11;
	Rectf srcRect, dstRect;
	dstRect = m_Shape;
	srcRect.height = m_pSpritesTexture->GetHeight() / nrRows;
	srcRect.width = (m_pSpritesTexture->GetWidth() - m_FrameOffset) / float(m_NrOfFrames);
	int currentColumn = m_AnimWaitMoveFrame % m_NrOfFrames;
	srcRect.bottom = -srcRect.height * m_CurrentRow;
	if (m_ActionState == ActionState::throwing)
	{
		currentColumn = m_AnimThrowFrame % m_NrOfFrames;
	}
	else if (m_ActionState == ActionState::dead)
	{
		currentColumn = m_AnimDeathFrame % m_NrOfFrames;
	}
	else if (m_ActionState == ActionState::takingdamage)
	{
		currentColumn = m_AnimDamageFrame % m_NrOfFrames;
	}
	else if (m_ActionState == ActionState::attacking)
	{
		int offset = 22;
		dstRect.left -= offset;
		currentColumn = m_AnimAttackFrame % m_NrOfFrames;
		srcRect.bottom = srcRect.height * m_CurrentRow;
	}
	srcRect.left = srcRect.width * currentColumn;
	dstRect.width = srcRect.width;
	dstRect.height = srcRect.height;
	//DrawAvatar
	Character::Draw(dstRect, srcRect);
	//Draw Throwable
	m_Throwable.Draw();
}

bool MeleeBoss::IsEnemyAttackingAvatar(const Rectf& rect) const
{
	if (utils::IsOverlapping(m_Shape, rect) && m_AnimAttackFrame >= m_NrOfSpecialAttackFrames / 2)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void MeleeBoss::SpecialAttack(float elapsedSec)
{
	const int triggerHealth = 60;
	const int multiplier = 2;
	if (m_Health < triggerHealth && !m_IsAttacking)
	{
		m_IsAttacking = true;
		m_ActionState = ActionState::attacking;
	}
	if (m_AnimAttackFrame >= m_NrOfSpecialAttackFrames / 2 && m_AnimAttackFrame != m_NrOfSpecialAttackFrames)
	{
		if (m_IsLookingLeft)
		{
			m_Velocity.x = -m_HorSpeed * multiplier;
		}
		else
		{
			m_Velocity.x = m_HorSpeed * multiplier;
		}
		m_Shape.left += m_Velocity.x * elapsedSec;
	}
	if (m_ActionState == ActionState::attacking)
	{
		m_IsHit = false;
	}
}

void MeleeBoss::ActionStates()
{
	switch (m_ActionState)
	{
		if (m_Throwable.GetIsThrowing())
		{
	case ActionState::throwing:
		m_FrameOffset = 168;
		m_NrOfFrames = 5;
		m_CurrentRow = 3;
		break;
		}
	case ActionState::moving:
		m_FrameOffset = 70;
		m_NrOfFrames = 6;
		m_CurrentRow = 0;
		break;
	case ActionState::waiting:
		m_FrameOffset = 184;
		m_NrOfFrames = 6;
		m_CurrentRow = 1;
		break;
	case ActionState::dead:
		m_FrameOffset = 150;
		m_NrOfFrames = 4;
		m_CurrentRow = 2;
		break;
	case ActionState::attacking:
		m_FrameOffset = 0;
		m_NrOfFrames = 5;
		m_CurrentRow = m_AnimAttackFrame / m_NrOfFrames + 1;
		break;
	case ActionState::takingdamage:
		m_FrameOffset = 328;
		m_NrOfFrames = 2;
		m_CurrentRow = 4;
		break;
	}
}

void MeleeBoss::ElapsedSecCount(float elapsedSec)
{
	Character::ElapsedSecCount(elapsedSec);
	//if specialattack
	if (m_ActionState == ActionState::attacking)
	{
		m_AnimWaitMoveFrame = 0;
		m_AnimThrowFrame = 0;
		m_AnimDamageFrame = 0;
		m_AnimTime += elapsedSec;
		if (m_AnimTime >= m_FrameChangeSpeed)
		{
			m_AnimAttackFrame++;
			if (m_AnimAttackFrame == m_NrOfSpecialAttackFrames / 2)
			{
				Sound::GetInstance()->PlaySoundEffect(14);
			}
			if (m_AnimAttackFrame == m_NrOfSpecialAttackFrames)
			{
				m_ActionState = ActionState::waiting;
			}
			m_AnimTime = 0;
		}
	}
}