#include "pch.h"
#include "RangeBoss.h"
#include "Enemy.h"
#include "Avatar.h"
#include <iostream>
#include "utils.h"

RangeBoss::RangeBoss(const Point2f& pos)
	:Enemy{ pos }
	, m_HasTeleported{ false }
	, m_AnimTeleportFrame{ 0 }
	, m_AnimDeathExplosionFrame{ 0 }
	, m_NrOfTeleportFrames{ 2 }
	, m_NrOfDeathFrames{ 4 }
	, m_NrOfDeathExplosionFrames{ 15 }
	, m_NrOfRows{ 5 }
	, m_pDeathExplosionTexture{ new Texture{"Resources/Images/EnemyDeathExplosion.png"} }
	, m_ShapeDeathExplosion{ Rectf{0,0,m_pDeathExplosionTexture->GetWidth() / 4.f, m_pDeathExplosionTexture->GetHeight() / 4.f} }
{
	m_Radius = { 800 };
	m_Offset = { 400 };
	m_pSpritesTexture = { new Texture{"Resources/Images/RangeBoss.png"} };
}

RangeBoss::~RangeBoss()
{
	delete m_pDeathExplosionTexture;
}

void RangeBoss::Update(float elapsedSec, const Level& level, Avatar* pAvatar, HUD& hud)
{
	if (IsDead() && !m_DoOnce)
	{
		hud.IncreaseScore(100);
		m_DoOnce = true;
	}
	//
	ElapsedSecCount(elapsedSec);
	ActionStates();
	Enemy::Update(elapsedSec, level, pAvatar, hud);
	if (m_HasTeleported)
	{
		m_IsThrowing = false;
	}
	Teleportation();
	UpdateExplosiveProjectiles(elapsedSec, pAvatar);
	//
	if (IsDeathExplosionHittingAvatar(pAvatar->GetShape()) && !m_HasAttacked)
	{
		pAvatar->OverlappingEnemySpecialAttackOrDeathExplosion(elapsedSec, m_IsLookingLeft);
		m_HasAttacked = true;
	}
	//
	m_ShapeDeathExplosion.left = m_Shape.left - (m_ShapeDeathExplosion.width / 2.f);
	m_ShapeDeathExplosion.bottom = m_Shape.bottom;
	//
}

void RangeBoss::Draw() const
{
	Rectf srcRect, dstRect;
	int currentColumn = 0;

	if (m_AnimDeathFrame != m_NrOfDeathFrames - 1)
	{
		int nrRows = 6;
		dstRect = m_Shape;
		srcRect.height = m_pSpritesTexture->GetHeight() / nrRows;
		srcRect.width = (m_pSpritesTexture->GetWidth() - m_FrameOffset) / float(m_NrOfFrames);
		srcRect.bottom = -srcRect.height * m_CurrentRow;
		currentColumn = m_AnimWaitMoveFrame % m_NrOfFrames;
		if (m_ActionState == ActionState::teleport)
		{
			currentColumn = m_AnimTeleportFrame % m_NrOfFrames; // if teleporting
		}
		else if (m_ActionState == ActionState::throwing)
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
		DrawExplosiveProjectiles();
		m_Throwable.Draw();
	}
	else
	{
		srcRect.width = m_pDeathExplosionTexture->GetWidth() / (m_NrOfDeathExplosionFrames / m_NrOfRows);
		srcRect.height = m_pDeathExplosionTexture->GetHeight() / m_NrOfRows;
		currentColumn = m_AnimDeathExplosionFrame % (m_NrOfDeathExplosionFrames / m_NrOfRows);
		int currentRow = m_AnimDeathExplosionFrame / (m_NrOfDeathExplosionFrames / m_NrOfRows) + 1;
		srcRect.left = srcRect.width * currentColumn;
		srcRect.bottom = srcRect.height * currentRow;
		dstRect = m_ShapeDeathExplosion;

		m_pDeathExplosionTexture->Draw(dstRect, srcRect);
	}
}

void RangeBoss::DrawExplosiveProjectiles() const
{
	for (size_t i = 0; i < m_ExplosiveProjectiles.size(); i++)
	{
		m_ExplosiveProjectiles[i]->Draw();
	}
}

void RangeBoss::UpdateExplosiveProjectiles(float elapsedSec, Avatar* pAvatar)
{
	const int maxTimer = 2;
	const int nrOfExplosionFrames = 7;
	const int triggerDistance = 80;
	if (!pAvatar->IsDead() && !IsDead() && m_HasTeleported && m_Delay > maxTimer)
	{
		m_ActionState = ActionState::throwing;
		m_ExplosiveProjectiles.push_back(new ExplosiveProjectile{ Point2f{m_Shape.left, m_Shape.bottom}, m_IsLookingLeft });
		m_Delay = 0;
	}
	for (size_t index = 0; index < m_ExplosiveProjectiles.size(); index++)
	{
		m_ExplosiveProjectiles[index]->Update(elapsedSec);
		//if overlaps with avatar
		if (m_ExplosiveProjectiles[index]->IsOverlapping(pAvatar->GetShape()))
		{
			pAvatar->OverlappingEnemyThrowableOrMelee(m_IsLookingLeft);
			m_ExplosiveProjectiles[index]->SetIsExploded(true);
			m_HasAttacked = true;
		}
		//out of range
		if (m_ExplosiveProjectiles[index]->GetAnimeExplosionFrame() >= nrOfExplosionFrames - 1)
		{
			delete m_ExplosiveProjectiles[index];
			m_ExplosiveProjectiles[index] = m_ExplosiveProjectiles[m_ExplosiveProjectiles.size() - 1];
			m_ExplosiveProjectiles.pop_back();
		}
		else if (abs(m_Shape.left - m_ExplosiveProjectiles[index]->GetShape().left) >= m_Radius || IsDead() || pAvatar->IsDead())
		{
			delete m_ExplosiveProjectiles[index];
			m_ExplosiveProjectiles[index] = m_ExplosiveProjectiles[m_ExplosiveProjectiles.size() - 1];
			m_ExplosiveProjectiles.pop_back();
		}
	}
	//states of projectile
	for (size_t index = 0; index < m_ExplosiveProjectiles.size(); index++)
	{
		if (abs(pAvatar->GetShape().left - m_ExplosiveProjectiles[index]->GetShape().left) < triggerDistance)
		{
			m_ExplosiveProjectiles[index]->SetExplosiveState(ExplosiveProjectile::ProjectileState::nearTarget);
		}
	}
}

void RangeBoss::Teleportation()
{
	const int limit = 40;
	if (m_Health <= limit && !m_HasTeleported)
	{
		m_ActionState = ActionState::teleport;
	}
	if (m_AnimTeleportFrame == m_NrOfTeleportFrames)
	{
		if (m_IsLookingLeft)
		{
			m_Shape.left += m_Radius - m_Offset;
		}
		else
		{
			m_Shape.left -= m_Radius + m_Offset;
		}
		m_AnimTeleportFrame = 0;
	}
}

void RangeBoss::ActionStates()
{
	switch (m_ActionState)
	{
		if (m_Throwable.GetIsThrowing())
		{
	case ActionState::throwing:
		m_FrameOffset = 82;
		m_NrOfFrames = 5;
		m_CurrentRow = 3;
		break;
		}
	case ActionState::moving:
		m_FrameOffset = 0;
		m_NrOfFrames = 6;
		m_CurrentRow = 0;
		break;
	case ActionState::waiting:
		m_FrameOffset = 177;
		m_NrOfFrames = 6;
		m_CurrentRow = 1;
		break;
	case ActionState::dead:
		m_FrameOffset = 120;
		m_NrOfFrames = 4;
		m_CurrentRow = 2;
		break;
	case ActionState::takingdamage:
		m_FrameOffset = 265;
		m_NrOfFrames = 2;
		m_CurrentRow = 4;
		break;
	case ActionState::teleport:
		m_FrameOffset = 282;
		m_CurrentRow = 5;
		break;
	}
}

void RangeBoss::ElapsedSecCount(float elapsedSec)
{
	Character::ElapsedSecCount(elapsedSec);
	//if attacking
	if (m_ActionState == ActionState::teleport)
	{
		const float multiplier = 1.5f;
		m_AnimWaitMoveFrame = 0;
		m_AnimDamageFrame = 0;
		m_AnimTime += elapsedSec;
		if (m_AnimTime >= m_FrameChangeSpeed * multiplier)
		{
			m_AnimTeleportFrame++;

			if (m_AnimTeleportFrame == m_NrOfTeleportFrames)
			{
				m_ActionState = ActionState::waiting;
				m_HasTeleported = true;
				Sound::GetInstance()->PlaySoundEffect(6);
			}
			m_AnimTime = 0;
		}
	}
	//if dead

	if (m_AnimDeathFrame == m_NrOfDeathFrames - 1)
	{
		m_AnimTime += elapsedSec;
		if (m_AnimTime >= m_FrameChangeSpeed)
		{
			m_AnimDeathExplosionFrame++;
			if (m_AnimDeathExplosionFrame == m_NrOfDeathExplosionFrames)
			{
				m_AnimDeathExplosionFrame = m_NrOfDeathExplosionFrames - 1;
			}
			m_AnimTime = 0;
		}
	}
}

bool RangeBoss::IsDeathExplosionHittingAvatar(const Rectf& rect) const
{
	int frameRange1 = 3;
	int frameRange2 = 10;
	if (utils::IsOverlapping(m_Shape, rect) && m_AnimDeathExplosionFrame >= frameRange1 && m_AnimDeathExplosionFrame <= frameRange2)
	{
		return true;
	}
	else
	{
		return false;
	}
}