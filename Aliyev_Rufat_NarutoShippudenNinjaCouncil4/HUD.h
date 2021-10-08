#pragma once
#include "Texture.h"
#include "structs.h"
class Avatar;

class HUD
{
public:
	HUD(const Rectf& window);
	void Draw() const;
	void Update(float elapsedSec, Avatar* pAvatar);
	void SetCounterTime(float counter);
	void IncreaseScore(int amountOfIncrease);
	void ResetScore();

private:
	static const int m_NrOfNumbers = 10;
	const int m_NrOfGoArrowFrames;
	const float m_FrameChangeSpeed;
	//
	Texture m_TextureHealthBar;
	Texture m_TextureLives;
	//
	Texture m_TextureArrayOfNumbers[m_NrOfNumbers];
	Texture m_TextureThrowablePickUp;
	//
	Texture m_TextureGoArrow;
	//
	Texture m_TextureMana;
	//
	Texture m_TextureHealthPickUp;
	//
	Texture m_TextureScore;
	////
	Point2f m_BottomLeftHealthBar;
	Point2f m_BottomLeftLives;
	Point2f m_BottomLeftGoArrow;
	//
	int m_Health;
	float m_Mana;
	int m_Lives;
	int m_Score;
	int m_AmountOfThrowables;
	int m_AmountOfHealthPickUps;
	//
	int m_AnimGoArrowFrame;
	float m_AnimTime;
	float m_Counter;
	//Methods
	void ElapsedSecCount(float elapsedSec);
	void DrawGoArrow() const;
	void DrawAmountOfPickUps() const;
	void DrawHealthAndManaBars() const;
	void DrawLives() const;
	void DrawScore() const;
};
