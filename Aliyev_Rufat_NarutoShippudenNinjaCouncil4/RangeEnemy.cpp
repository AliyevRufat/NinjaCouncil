#include "pch.h"
#include "RangeEnemy.h"
#include "utils.h"

RangeEnemy::RangeEnemy(const Point2f& pos)
	:Enemy{ pos }
{
	m_Health = 60;
	m_MaxHealth = 60;
	m_Radius = { 300 };
	m_Offset = { 150 };
	m_pSpritesTexture = { new Texture{"Resources/Images/RangeEnemy.png"} };
}

void RangeEnemy::Update(float elapsedSec, const Level& level, Avatar* pAvatar, HUD& hud)
{
	if (IsDead() && !m_DoOnce)
	{
		hud.IncreaseScore(20);
		m_DoOnce = true;
	}
	//
	Character::ElapsedSecCount(elapsedSec);
	ActionStates();
	Enemy::Update(elapsedSec, level, pAvatar, hud);
}

void RangeEnemy::Draw() const
{
	Rectf srcRect, dstRect;
	int nrRows = 5;
	int currentColumn = m_AnimWaitMoveFrame % m_NrOfFrames;
	dstRect = m_Shape;
	srcRect.height = m_pSpritesTexture->GetHeight() / nrRows;
	srcRect.width = (m_pSpritesTexture->GetWidth() - m_FrameOffset) / float(m_NrOfFrames);
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
	srcRect.left = srcRect.width * currentColumn;
	dstRect.width = srcRect.width;
	dstRect.height = srcRect.height;
	Character::Draw(dstRect, srcRect);
	m_Throwable.Draw();
}

void RangeEnemy::ActionStates()
{
	switch (m_ActionState)
	{
		if (m_Throwable.GetIsThrowing())
		{
	case ActionState::throwing:
		m_FrameOffset = 60;
		m_NrOfFrames = 4;
		m_CurrentRow = 4;
		break;
		}
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