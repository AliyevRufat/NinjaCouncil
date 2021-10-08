#include "pch.h"
#include "pch.h"
#include "Avatar.h"
#include "utils.h"
#include <iostream>
#include "Character.h"
#include "Sound.h"

Avatar::Avatar(const Point2f& pos)
	: m_Throwable{ m_StartingAmountOfThrowables }
	, m_IsOnGround{ false }
	, m_ComboAttack{ false }
	, m_ComboAttack2{ false }
	, m_IsJumping{ false }
	, m_IsFalling{ false }
	, m_IsTakingDamageFromLeft{ false }
	, m_IsInvisible{ false }
	, m_InvisibleSwitch{ false }
	, m_IsMovingToNewLevel{ false }
	, m_CanSpecialAttack{ false }
	, m_CanPlayDeathSE{ true }
	, m_AccumlatedLostHealth{ 0 }
	, m_AnimSpecialAttackFrame{ 0 }
	, m_JumpCooldownTime{ 0.f }
	, m_JumpCooldownMax{ 0.05f }
	, m_AnimJumpFrame{ 0 }
	, m_AmountOfHealthPickUps{ 0 }
	, m_Lives{ 2 }
	, m_MaxLives{ 2 }
	, m_Counter{ 0.f }
	, m_MaxSecInvisible{ 3.f }
	, m_PlatformCollisionPosY{ 0 }
	, m_Mana{ 210 }
	, m_MaxMana{ 210 }
{
	m_pSpritesTexture = { new Texture{"Resources/Images/Avatar.png"} };
	m_Shape = { Rectf{pos.x,pos.y,36,60} };
	m_TranslateOffset = { 20 };
	m_Health = 125;
	m_MaxHealth = 125;
}

void Avatar::Draw() const
{
	//Destination Rect and Source Rect
	const int nrRows = 10;
	const int frameOffsetForSpecialAttack = 1040;
	//
	Rectf srcRect, dstRect;
	dstRect = m_Shape;
	srcRect.height = (m_pSpritesTexture->GetHeight() - 1040) / nrRows;
	srcRect.width = (m_pSpritesTexture->GetWidth() - m_FrameOffset) / m_NrOfFrames;
	int currentColumn = m_AnimWaitMoveFrame % m_NrOfFrames;
	srcRect.bottom = -srcRect.height * m_CurrentRow;

	if (m_ActionState == ActionState::attacking)
	{
		currentColumn = m_AnimAttackFrame % m_NrOfFrames; // if attacking
	}
	else if (m_ActionState == ActionState::throwing)
	{
		currentColumn = m_AnimThrowFrame % m_NrOfFrames; // if throwing
	}
	else if (m_ActionState == ActionState::dead)
	{
		currentColumn = m_AnimDeathFrame % m_NrOfFrames; // if dead
	}
	else if (m_ActionState == ActionState::takingdamage)
	{
		currentColumn = m_AnimDamageFrame % m_NrOfFrames; // if takingdamage
	}
	else if (m_ActionState == ActionState::jumping) // if jumping
	{
		currentColumn = m_AnimJumpFrame % m_NrOfFrames;
	}
	else if (m_ActionState == ActionState::specialattack)
	{
		const float nrOfSpecialAttackRows = 12.423f;
		int offset = 31;
		//
		srcRect.height = m_pSpritesTexture->GetHeight() / nrOfSpecialAttackRows;
		srcRect.width = m_pSpritesTexture->GetWidth() / m_NrOfFrames;
		dstRect.left -= offset;
		currentColumn = m_AnimSpecialAttackFrame % m_NrOfFrames;
		srcRect.bottom = srcRect.height * m_CurrentRow;
	}
	srcRect.left = srcRect.width * currentColumn;
	dstRect.width = srcRect.width;
	dstRect.height = srcRect.height;
	//Draw avatar
	if (!m_InvisibleSwitch)
	{
		Character::Draw(dstRect, srcRect);
	}
	//Draw Throwable
	m_Throwable.Draw();
}

void Avatar::Update(float elapsedSec, const Level& level)
{
	if (level.IsOnGround(m_Shape, m_Velocity))
	{
		m_IsOnGround = true;
	}
	else
	{
		m_IsOnGround = false;
	}
	//dropping out of the level
	if (m_Shape.bottom <= -m_Shape.height)
	{
		m_Health = 0;
		m_CanSpecialAttack = false;
		m_AnimSpecialAttackFrame = 0;
	}
	//Frame Change (animations)
	ElapsedSecCount(elapsedSec);
	//ActionStates
	ActionStates();
	//Checks Jumping
	Jumping(elapsedSec, level);
	//CollisionCheck
	level.HandleCollision(m_Shape, m_Velocity);
	if (m_Health != 0)
	{
		//Keys (moving, jumping and stopping)
		HandleInput(elapsedSec, level);
		// x and y axis move
		if (!IsDead())
		{
			m_Shape.left += m_Velocity.x * elapsedSec;
			m_Shape.bottom += m_Velocity.y * elapsedSec;
			m_Velocity += m_Acceleration * elapsedSec;
		}
		//Is waiting
		if (m_ActionState != ActionState::attacking && m_ActionState != ActionState::throwing && m_ActionState != ActionState::specialattack)
		{
			const Uint8* pStates = SDL_GetKeyboardState(nullptr);
			if (!(pStates[SDL_SCANCODE_RIGHT] || pStates[SDL_SCANCODE_LEFT] || pStates[SDL_SCANCODE_UP]) && m_IsOnGround)
			{
				m_ActionState = ActionState::waiting;
			}
		}
	}
	m_Throwable.IsNotRange(m_Shape);
	m_Throwable.Update(elapsedSec);
	//
	TakingDamage();
	//
	InvisibleState(elapsedSec);
	//
	// mana ++
	if (m_Mana < m_MaxMana)
	{
		const int increaseRate = 10;
		m_Mana += increaseRate * elapsedSec;
	}
	//
	SpecialAttack();
}

void Avatar::SpecialAttack()
{
	if (m_CanSpecialAttack)
	{
		m_Mana = 0;
		m_ActionState = ActionState::specialattack;
		m_IsHit = false;
		m_Velocity.x = 0;
	}
	//attackframes to move before attacking
	const int specialAttackMoveFrame = 15;
	const int specialAttackStopFrame = 21;
	const float doubleHorSpeed = m_HorSpeed * 2;
	//
	if (m_AnimSpecialAttackFrame >= specialAttackMoveFrame && m_AnimSpecialAttackFrame <= specialAttackStopFrame)
	{
		if (m_IsLookingLeft)
		{
			m_Velocity.x = -doubleHorSpeed;
		}
		else
		{
			m_Velocity.x = doubleHorSpeed;
		}
	}
	else if (m_AnimSpecialAttackFrame == specialAttackStopFrame + 1)
	{
		Sound::GetInstance()->PlaySoundEffect(8);
		m_Velocity.x = 0;
	}
}

void Avatar::ProcessKeyDownEvent(const SDL_KeyboardEvent& e)
{
	if (m_ActionState != ActionState::takingdamage && m_ActionState != ActionState::dead && !m_IsMovingToNewLevel)
	{
		switch (e.keysym.sym)
		{
		case SDLK_q:
			if (!m_IsInvisible && m_ActionState == ActionState::waiting && !m_Throwable.GetIsThrowing() && m_Throwable.GetAmountOfThrowables() > 0 && m_ActionState != ActionState::specialattack)
			{
				m_ActionState = ActionState::throwing;
				m_Throwable.DecreaseThrowables();
				m_Throwable.IsLookingLeft(m_IsLookingLeft);
				m_Throwable.SetPosition(Point2f{ m_Shape.left, m_Shape.bottom });
				m_Throwable.SetIsThrowing(true);
				m_IsThrowing = true;
				Sound::GetInstance()->PlaySoundEffect(10);
			}
			break;
		case SDLK_a:
			if (!m_IsInvisible && m_AnimAttackFrame == 0 && m_ActionState != ActionState::moving && m_ActionState != ActionState::throwing && m_ActionState != ActionState::jumping && m_ActionState != ActionState::specialattack)
			{
				m_IsAttacking = true;

				if (!m_ComboAttack2 && !m_ComboAttack)
				{
					m_ActionState = ActionState::attacking;
					m_ComboAttack = true;
					Sound::GetInstance()->PlaySoundEffect(0);
				}
				else if (m_ComboAttack && !m_ComboAttack2)
				{
					m_ActionState = ActionState::attacking;
					m_ComboAttack = false;
					m_ComboAttack2 = true;
					Sound::GetInstance()->PlaySoundEffect(1);
				}
				else if (m_ComboAttack2)
				{
					m_ActionState = ActionState::attacking;
					m_ComboAttack2 = false;
					Sound::GetInstance()->PlaySoundEffect(2);
				}
			}
			break;
		case SDLK_h:
			if (m_Health != m_MaxHealth && m_AmountOfHealthPickUps > 0)
			{
				IncreaseHealth();
				--m_AmountOfHealthPickUps;
			}
			break;
		case SDLK_s:
			if (!m_IsInvisible && !m_CanSpecialAttack)
			{
				if (m_Mana >= m_MaxMana && (m_ActionState == ActionState::waiting || m_ActionState == ActionState::moving))
				{
					m_CanSpecialAttack = true;
					Sound::GetInstance()->PlaySoundEffect(7);
				}
				else
				{
					m_CanSpecialAttack = false;
				}
			}
			break;
		}
	}
}

void Avatar::OverlappingEnemySpecialAttackOrDeathExplosion(float elapsedSec, bool isLookingLeft)
{
	if (!IsDead())
	{
		const int damage = 40;
		m_AccumlatedLostHealth += damage;
		m_IsTakingDamage = true;
		m_IsTakingDamageFromLeft = isLookingLeft;
		m_Health -= damage;
		if (m_Health <= 0)
		{
			m_Health = 0;
		}
	}
}

void Avatar::OverlappingEnemyThrowableOrMelee(bool isLookingLeft)
{
	if (!IsDead() && !m_IsInvisible && !m_CanSpecialAttack)
	{
		m_IsHit = true;
		m_IsTakingDamage = true;
		m_IsTakingDamageFromLeft = isLookingLeft;
	}
}

bool Avatar::IsEnemyOverlappingThrowable(const Rectf& rect)
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

bool Avatar::IsEnemyOverlappingSpecialAttack(const Rectf& rect) const
{
	if (utils::IsOverlapping(m_Shape, rect) && m_ActionState == ActionState::specialattack)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Avatar::IsDead()
{
	if (m_Health <= 0)
	{
		if (m_CanPlayDeathSE)
		{
			Sound::GetInstance()->PlaySoundEffect(11);
			m_CanPlayDeathSE = false;
		}
		m_ActionState = ActionState::dead;
		m_Health = 0;
		return true;
	}
	return false;
}

void Avatar::Respawn()
{
	m_Velocity.y = 0;
	m_Mana = m_MaxMana;
	--m_Lives;
	if (m_Lives <= 0)
	{
		m_Lives = 0;
	}
	m_AmountOfHealthPickUps = 0;
	m_IsTakingDamage = false;
	m_Health = m_MaxHealth;
	m_CanPlayDeathSE = true;
	m_AccumlatedLostHealth = 0;
}

void Avatar::Restart()
{
	m_Velocity.y = 0;
	m_Lives = m_MaxLives;
	m_Mana = m_MaxMana;
	m_AmountOfHealthPickUps = 0;
	m_IsTakingDamage = false;
	m_Health = m_MaxHealth;
	m_Throwable.SetAmountOfThrowables(m_StartingAmountOfThrowables);
	m_CanSpecialAttack = false;
	m_AnimSpecialAttackFrame = 0;
	m_CanPlayDeathSE = true;
	m_AccumlatedLostHealth = 0;
}

void Avatar::IncreaseAmountOfPickUps()
{
	m_AmountOfHealthPickUps++;
}

void Avatar::IncreaseThrowables()
{
	m_Throwable.IncreaseThrowables();
}

bool Avatar::GetIsHit() const
{
	if (m_IsHit || m_ActionState == ActionState::takingdamage)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Avatar::GetIsAttacking() const
{
	return m_IsAttacking;
}

void Avatar::SetIsAttacking(bool isAttacking)
{
	m_IsAttacking = isAttacking;
}

void Avatar::HandleInput(float elapsedSec, const Level& level)
{
	if (!m_IsAttacking && !m_IsThrowing && !m_IsTakingDamage && !m_CanSpecialAttack)
	{
		const Uint8* pStates = SDL_GetKeyboardState(nullptr);

		if (!m_IsMovingToNewLevel)
		{
			if (pStates[SDL_SCANCODE_RIGHT])
			{
				m_IsLookingLeft = false;
				m_Velocity.x = m_HorSpeed;
				if (!m_IsJumping && !m_IsFalling)
				{
					m_ActionState = ActionState::moving;
				}
			}
			else if (pStates[SDL_SCANCODE_LEFT])
			{
				m_IsLookingLeft = true;
				m_Velocity.x = -m_HorSpeed;
				if (!m_IsJumping && !m_IsFalling)
				{
					m_ActionState = ActionState::moving;
				}
			}

			if (pStates[SDL_SCANCODE_UP] && m_JumpCooldownTime >= m_JumpCooldownMax && m_ActionState != ActionState::specialattack && level.IsOnGround(m_Shape, m_Velocity))
			{
				Sound::GetInstance()->PlaySoundEffect(9);
				m_Velocity.y = m_JumpSpeed;
				m_JumpCooldownTime = 0;
				m_ActionState = ActionState::jumping;
			}
		}
		//slowing down
		if (m_IsMovingToNewLevel || (!pStates[SDL_SCANCODE_RIGHT] && !pStates[SDL_SCANCODE_LEFT]))
		{
			//inertia/sliding
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
		}
	}
}

void Avatar::ActionStates()
{
	switch (m_ActionState)
	{
	case ActionState::specialattack:
		m_FrameOffset = 0;
		m_NrOfFrames = 5;
		m_CurrentRow = m_AnimSpecialAttackFrame / m_NrOfFrames + 1;
		break;
	case ActionState::throwing:
		m_FrameOffset = 603;
		m_NrOfFrames = 5;
		m_CurrentRow = 6;
		break;
	case ActionState::moving:
		m_FrameOffset = 543;
		m_NrOfFrames = 6;
		m_CurrentRow = 4;
		break;
	case ActionState::attacking:
		//combo attack
		if (m_ComboAttack)
		{
			m_FrameOffset = 662;
			m_NrOfFrames = 4;
			m_CurrentRow = 2;
		}
		else if (m_ComboAttack2)
		{
			m_FrameOffset = 662;
			m_NrOfFrames = 4;
			m_CurrentRow = 1;
		}
		else
		{
			m_FrameOffset = 662;
			m_NrOfFrames = 4;
			m_CurrentRow = 0;
		}
		break;
	case ActionState::waiting:
		m_FrameOffset = 636;
		m_NrOfFrames = 6;
		m_CurrentRow = 3;
		break;
	case ActionState::takingdamage:
		m_FrameOffset = 781;
		m_NrOfFrames = 2;
		m_CurrentRow = 7;
		break;
	case ActionState::dead:
		m_FrameOffset = 662;
		m_NrOfFrames = 4;
		m_CurrentRow = 8;
		break;
	case ActionState::jumping:
		if (m_IsJumping)
		{
			m_Acceleration.y = -981.f;
			m_FrameOffset = 830;
			m_NrOfFrames = 2;
			m_CurrentRow = 5;
		}
		else if (m_IsFalling)
		{
			m_Acceleration.y = -400.f;
			m_FrameOffset = 798;
			m_NrOfFrames = 2;
			m_CurrentRow = 9;
		}
		break;
	}
}

void Avatar::ElapsedSecCount(float elapsedSec)
{
	Character::ElapsedSecCount(elapsedSec);

	if (m_ActionState == ActionState::attacking)
	{
		m_AnimWaitMoveFrame = 0;
		m_AnimThrowFrame = 0;
		m_AnimDamageFrame = 0;
		m_AnimJumpFrame = 0;
		m_AnimTime += elapsedSec;
		if (m_AnimTime >= m_FrameChangeSpeed)
		{
			m_AnimAttackFrame++;
			if (m_AnimAttackFrame == m_NrOfAttackFrames)
			{
				m_AnimAttackFrame = 0;
				m_IsAttacking = false;
				m_ActionState = ActionState::waiting;
			}
			m_AnimTime = 0;
		}
	}
	else if (m_ActionState == ActionState::jumping)
	{
		m_AnimWaitMoveFrame = 0;
		m_AnimThrowFrame = 0;
		m_AnimAttackFrame = 0;
		m_AnimDamageFrame = 0;
		m_AnimTime += elapsedSec;
		if (m_AnimTime >= m_FrameChangeSpeed)
		{
			m_AnimJumpFrame++;
			if (m_AnimJumpFrame == m_NrOfJumpFrames)
			{
				m_AnimJumpFrame = 0;
			}
			m_AnimTime = 0;
		}
	}
	else if (m_ActionState == ActionState::specialattack)
	{
		m_AnimWaitMoveFrame = 0;
		m_AnimThrowFrame = 0;
		m_AnimAttackFrame = 0;
		m_AnimDamageFrame = 0;
		m_AnimTime += elapsedSec;
		if (m_AnimTime >= m_FrameChangeSpeed)
		{
			m_AnimSpecialAttackFrame++;

			if (m_AnimSpecialAttackFrame >= m_NrOfSpecialAttackFrames)
			{
				m_AnimSpecialAttackFrame = 0;
				m_ActionState = ActionState::waiting;
				m_CanSpecialAttack = false;
			}
			m_AnimTime = 0;
		}
	}
}

void Avatar::IncreaseHealth()
{
	int gainHealth = 20;
	if (m_Health < m_MaxHealth)
	{
		m_Health += gainHealth;
		if (m_Health > m_MaxHealth)
		{
			m_Health = m_MaxHealth;
		}
	}
}

void Avatar::TakingDamage()
{
	const int m_RandMax = 2;

	if (m_IsHit && !m_IsInvisible)
	{
		int damage = 20;
		int randNr = rand() % m_RandMax;
		//
		m_AccumlatedLostHealth += damage;
		if (randNr == 0)
		{
			Sound::GetInstance()->PlaySoundEffect(3);
		}
		else
		{
			Sound::GetInstance()->PlaySoundEffect(4);
		}
		m_Health -= damage;
	}
	m_IsHit = false;
	//
	if (m_IsTakingDamage)
	{
		float pushSpeed = 70.0f;
		m_ActionState = ActionState::takingdamage;
		if (m_IsTakingDamageFromLeft)
		{
			m_Velocity.x = -pushSpeed;
		}
		else
		{
			m_Velocity.x = pushSpeed;
		}
		m_Velocity.y = 0;
		m_IsAttacking = false;
		m_IsThrowing = false;
		if (m_AccumlatedLostHealth >= m_MaxHealth / m_RandMax && m_Lives > 0)
		{
			m_IsInvisible = true;
			m_InvisibleSwitch = true;
			m_AccumlatedLostHealth -= m_AccumlatedLostHealth;
		}
	}
}

void Avatar::Jumping(float elapsedSec, const Level& level)
{
	if (!m_IsOnGround && m_Velocity.y > 0)
	{
		m_ActionState = ActionState::jumping;
		m_IsJumping = true;
		m_IsFalling = false;
	}
	else if (!m_IsOnGround && m_Velocity.y < 0)
	{
		m_ActionState = ActionState::jumping;
		m_IsFalling = true;
		m_IsJumping = false;
	}
	else
	{
		m_JumpCooldownTime += elapsedSec;
		m_IsJumping = false;
		m_IsFalling = false;
	}
}

void Avatar::InvisibleState(float elapsedSec)
{
	if (m_IsInvisible)
	{
		m_InvisibleSwitch = !m_InvisibleSwitch;
		m_Counter += elapsedSec;
		if (m_Counter >= m_MaxSecInvisible)
		{
			m_IsInvisible = false;
			m_InvisibleSwitch = false;
			m_Counter -= m_Counter;
		}
	}
}

int Avatar::GetHealth()const
{
	return m_Health;
}

float Avatar::GetMana()const
{
	return m_Mana;
}

int Avatar::GetLives()const
{
	return m_Lives;
}

int Avatar::GetAmountOfHealthPickUps()const
{
	return m_AmountOfHealthPickUps;
}

int Avatar::GetAmountOfThrowables()const
{
	return m_Throwable.GetAmountOfThrowables();
}

int Avatar::GetSpecialAttackFrame()const
{
	return m_AnimSpecialAttackFrame;
}

void Avatar::SetIsMovingToNewLevel(bool isMovingToNewLevel)
{
	m_IsMovingToNewLevel = isMovingToNewLevel;
}

void Avatar::SetVelocity(int xVelocity, float elapsedSec)
{
	m_Shape.left += xVelocity * elapsedSec;
	if (xVelocity > 0)
	{
		m_ActionState = ActionState::moving;
	}
}