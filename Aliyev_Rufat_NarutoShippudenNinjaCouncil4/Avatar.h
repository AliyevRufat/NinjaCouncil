#pragma once
#include "Level.h"
#include "Character.h"
#include "Throwable.h"
#include "EnemyManager.h"
#include "Sound.h"

class Avatar final : public Character
{
public:
	Avatar(const Point2f& pos);
	void Draw() const;
	void Update(float elapsedSec, const Level& level);
	void ProcessKeyDownEvent(const SDL_KeyboardEvent& e);
	//collisions
	void OverlappingEnemyThrowableOrMelee(bool isLookingLeft);
	void OverlappingEnemySpecialAttackOrDeathExplosion(float elapsedSec, bool isLookingLeft);
	bool IsEnemyOverlappingSpecialAttack(const Rectf& rect) const;
	bool IsEnemyOverlappingThrowable(const Rectf& rect);
	//
	bool IsDead();
	void Respawn();
	void Restart();
	void IncreaseAmountOfPickUps();
	void IncreaseThrowables();
	bool GetIsHit() const;
	void SetIsAttacking(bool isAttacking);
	bool GetIsAttacking() const;
	void SetVelocity(int xVelocity, float elapsedSec);
	void SetIsMovingToNewLevel(bool isMovingToNewLevel);

	//for HUD
	int GetHealth()const;
	float GetMana()const;
	int GetLives()const;
	int GetAmountOfHealthPickUps()const;
	int GetAmountOfThrowables()const;
	int GetSpecialAttackFrame()const;

private:
	//Methods
	void ElapsedSecCount(float elapsedSec) override;
	void ActionStates();
	void Jumping(float elapsedSec, const Level& level);
	void HandleInput(float elapsedSec, const Level& level);
	void TakingDamage();
	void InvisibleState(float elapsedSec);
	void IncreaseHealth();
	void SpecialAttack();

	//Datamembers
	const int m_StartingAmountOfThrowables = 20;
	const int m_NrOfAttackFrames = 4;
	const int m_NrOfJumpFrames = 2;
	const int m_NrOfSpecialAttackFrames = 40;
	Throwable m_Throwable;
	//
	bool m_IsOnGround;
	bool m_ComboAttack;
	bool m_ComboAttack2;
	bool m_IsFalling;
	bool m_IsJumping;
	bool m_IsTakingDamageFromLeft;
	bool m_IsInvisible;
	bool m_InvisibleSwitch;
	bool m_CanPlayDeathSE;
	bool m_IsMovingToNewLevel;
	//
	float m_JumpCooldownTime;
	const float m_JumpCooldownMax;
	float m_Counter;
	const float m_MaxSecInvisible;
	int m_AnimJumpFrame;
	//
	int m_Lives;
	float m_Mana;
	const float m_MaxMana;
	const int m_MaxLives;
	int m_AmountOfHealthPickUps;
	int m_PlatformCollisionPosY;
	int m_AnimSpecialAttackFrame;
	int m_AccumlatedLostHealth;
	//
	bool m_CanSpecialAttack;
};