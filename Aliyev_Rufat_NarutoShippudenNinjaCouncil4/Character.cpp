#include "pch.h"
#include "Character.h"
#include <iostream>

Character::Character()
	:m_Shape{ Rectf{0,0,0,0} }
	, m_SrcRect{ Rectf{0,0,0,0} }
	, m_pSpritesTexture{ nullptr }
	, m_HorSpeed{ 250 }
	, m_JumpSpeed{ 450 }
	, m_Velocity{ Vector2f{0.0f,0.0f} }
	, m_Acceleration{ Vector2f{0,-981.0f} }
	, m_AnimTime{ 0 }
	, m_AnimWaitMoveFrame{ 0 }
	, m_AnimThrowFrame{ 0 }
	, m_AnimAttackFrame{ 0 }
	, m_AnimDeathFrame{ 0 }
	, m_AnimDamageFrame{ 0 }
	, m_CurrentRow{ 0 }
	, m_IsLookingLeft{ false }
	, m_IsThrowing{ false }
	, m_IsHit{ false }
	, m_IsTakingDamage{ false }
	, m_IsAttacking{ false }
	, m_NrOfFrames{ 6 }
	, m_FrameOffset{ 0 }
	, m_FrameChangeSpeed{ 0.1f }
	, m_ActionState{ ActionState::waiting }
	, m_Health{ 100 }
	, m_MaxHealth{ 100 }
	, m_TranslateOffset{ 0 }
{
}

Character::~Character()
{
	delete m_pSpritesTexture;
	m_pSpritesTexture = nullptr;
}

void Character::SetPos(const Point2f& pos)
{
	m_Shape.left = pos.x;
	m_Shape.bottom = pos.y;
}

void Character::Draw(const Rectf& dstRect, const Rectf& srcRect) const
{
	if (m_IsLookingLeft)
	{
		const float doubleShapeLeft = m_Shape.left * 2.f;
		glPushMatrix();
		glTranslatef(doubleShapeLeft + m_Shape.width + m_TranslateOffset, 0, 0);
		glScalef(-1, 1, 0);
		m_pSpritesTexture->Draw(dstRect, srcRect);
		glPopMatrix();
	}
	else
	{
		glPushMatrix();
		glScalef(1, 1, 0);
		m_pSpritesTexture->Draw(dstRect, srcRect);
		glPopMatrix();
	}
}

Rectf Character::GetShape() const
{
	return m_Shape;
}

void Character::ElapsedSecCount(float elapsedSec)
{
	if (m_ActionState == ActionState::waiting || m_ActionState == ActionState::moving)
	{
		m_AnimThrowFrame = 0;
		m_AnimAttackFrame = 0;
		m_AnimTime += elapsedSec;
		if (m_AnimTime >= m_FrameChangeSpeed)
		{
			m_AnimWaitMoveFrame++;
			if (m_AnimWaitMoveFrame == m_NrOfFrames)
			{
				m_AnimWaitMoveFrame = 0;
			}
			m_AnimTime = 0;
		}
	}
	else if (m_ActionState == ActionState::throwing)
	{
		m_AnimWaitMoveFrame = 0;
		m_AnimAttackFrame = 0;
		m_AnimTime += elapsedSec;

		if (m_AnimTime >= m_FrameChangeSpeed)
		{
			m_AnimThrowFrame++;
			if (m_AnimThrowFrame == m_NrOfFrames)
			{
				m_AnimThrowFrame = 0;
				m_IsThrowing = false;
				m_ActionState = ActionState::waiting;
			}
			m_AnimTime = 0;
		}
	}
	else if (m_ActionState == ActionState::dead)
	{
		m_AnimWaitMoveFrame = 0;
		m_AnimThrowFrame = 0;
		m_AnimTime += elapsedSec;
		if (m_AnimTime >= m_FrameChangeSpeed)
		{
			m_AnimDeathFrame++;
			if (m_AnimDeathFrame >= m_NrOfFrames)
			{
				m_AnimDeathFrame = m_NrOfFrames - 1;
			}
			m_AnimTime = 0;
		}
	}
	else if (m_ActionState == ActionState::takingdamage)
	{
		m_AnimWaitMoveFrame = 0;
		m_AnimThrowFrame = 0;
		m_AnimAttackFrame = 0;
		m_AnimTime += elapsedSec;
		const float doubleFrameChangeSpeed = m_FrameChangeSpeed * 2;
		if (m_AnimTime >= doubleFrameChangeSpeed)
		{
			m_AnimDamageFrame++;
			if (m_AnimDamageFrame >= m_NrOfFrames)
			{
				m_AnimDamageFrame = 0;
				m_ActionState = ActionState::waiting;
				m_IsTakingDamage = false;
			}
			m_AnimTime = 0;
		}
	}
}