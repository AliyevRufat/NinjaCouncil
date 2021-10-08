#pragma once
#include "Level.h"
#include "SoundEffect.h"

class Character
{
public:

	enum class ActionState
	{
		waiting = 0,
		moving = 1,
		attacking = 2,
		throwing = 3,
		dead = 4,
		takingdamage = 5,
		jumping = 6,
		teleport = 7,
		specialattack = 8
	};
	//Public Methods
	explicit Character();
	virtual ~Character();
	Character(const Character&) = delete;
	Character& operator=(const Character&) = delete;
	Character(Character&&) = delete;
	Character& operator=(Character&&) = delete;
	//--
	void Draw(const Rectf& dstRect, const Rectf& srcRect) const;
	Rectf GetShape() const;
	void SetPos(const Point2f& pos);
protected:
	//Methods
	virtual void ElapsedSecCount(float elapsedSec);
	//DataMembers
	//--
	Texture* m_pSpritesTexture;
	ActionState m_ActionState;
	//--
	int m_NrOfFrames; //changes
	int m_FrameOffset; //changes
	const float m_FrameChangeSpeed;
	//--
	float m_AnimTime;
	int m_AnimWaitMoveFrame;
	int m_AnimThrowFrame;
	int m_AnimAttackFrame;
	int m_AnimDeathFrame;
	int m_AnimDamageFrame;
	//--
	int m_TranslateOffset;
	//--
	int m_CurrentRow;
	//--
	Rectf m_SrcRect;
	Rectf m_Shape;
	//--
	float m_HorSpeed;
	const float m_JumpSpeed;
	//--
	Vector2f m_Velocity;
	Vector2f m_Acceleration; //changes
	//--
	bool m_IsLookingLeft;
	bool m_IsThrowing;
	bool m_IsHit;
	bool m_IsTakingDamage;
	bool m_IsAttacking;
	//--
	int m_Health;
	int m_MaxHealth; // changes
	//
};
