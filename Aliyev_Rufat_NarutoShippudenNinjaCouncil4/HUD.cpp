#include "pch.h"
#include "HUD.h"
#include "utils.h"
#include <iostream>
#include "Avatar.h"

HUD::HUD(const Rectf& window)
	:m_BottomLeftHealthBar{ Point2f{ window.width / 25,window.height - 70 } }
	, m_BottomLeftLives{ Point2f{ window.width / 25 , window.height / 30 } }
	, m_BottomLeftGoArrow{ Point2f{ window.width - window.width / 4.f , window.height - window.height / 3 } }
	, m_TextureHealthBar{ "Resources/Images/HUDHealthBar.png" }
	, m_TextureMana{ "Resources/Images/HUDMana.png" }
	, m_TextureLives{ "Resources/Images/HUDLives.png" }
	, m_TextureThrowablePickUp{ "Resources/Images/HUDWeapon.png" }
	, m_TextureGoArrow{ "Resources/Images/HUDGoArrow.png" }
	, m_TextureHealthPickUp{ "Resources/Images/HUDHealth.png" }
	, m_TextureScore{ "Resources/Images/HUDScore.png" }
	, m_NrOfGoArrowFrames{ 6 }
	, m_AnimGoArrowFrame{ 0 }
	, m_FrameChangeSpeed{ 0.08f }
	, m_Health{}
	, m_Mana{}
	, m_Lives{}
	, m_AmountOfThrowables{}
	, m_AmountOfHealthPickUps{ 0 }
	, m_AnimTime{ 0 }
	, m_Counter{ 0 }
	, m_Score{ 0 }
	, m_TextureArrayOfNumbers
{
	Texture("Resources/Images/NumberZero.png")
	,Texture("Resources/Images/NumberOne.png")
	,Texture("Resources/Images/NumberTwo.png")
	,Texture("Resources/Images/NumberThree.png")
	,Texture("Resources/Images/NumberFour.png")
	,Texture("Resources/Images/NumberFive.png")
	,Texture("Resources/Images/NumberSix.png")
	,Texture("Resources/Images/NumberSeven.png")
	,Texture("Resources/Images/NumberEight.png")
	,Texture("Resources/Images/NumberNine.png")
}
{
}

void HUD::Draw() const
{
	DrawGoArrow();
	DrawAmountOfPickUps();
	DrawHealthAndManaBars();
	DrawLives();
	DrawScore();
}

void HUD::DrawScore() const
{
	//score texture
	const int offset = 30;
	Rectf dstRectfScore = Rectf{ m_BottomLeftLives.x + offset + m_TextureLives.GetWidth() ,m_BottomLeftLives.y , m_TextureScore.GetWidth() , m_TextureScore.GetHeight() };
	m_TextureScore.Draw(dstRectfScore);
	//amount of score
	const int intDivisionDigit10 = 10;
	const int intDivisionDigit100 = 100;
	Rectf dstRectfAmountOfScore{ Rectf{ dstRectfScore.left + m_TextureScore.GetWidth() + m_TextureArrayOfNumbers[1].GetWidth() ,dstRectfScore.bottom,m_TextureArrayOfNumbers[1].GetWidth()  , m_TextureArrayOfNumbers[1].GetHeight() } };
	if (m_Score < intDivisionDigit10)
	{
		m_TextureArrayOfNumbers[m_Score].Draw(dstRectfAmountOfScore);
	}
	else if (m_Score >= intDivisionDigit10 && m_Score < intDivisionDigit100)
	{
		//first number
		m_TextureArrayOfNumbers[m_Score / intDivisionDigit10].Draw(dstRectfAmountOfScore);
		//second number
		dstRectfAmountOfScore = { Rectf{  dstRectfScore.left + m_TextureScore.GetWidth() + m_TextureArrayOfNumbers[1].GetWidth() * 2, dstRectfScore.bottom,m_TextureArrayOfNumbers[1].GetWidth()  , m_TextureArrayOfNumbers[1].GetHeight() } };
		m_TextureArrayOfNumbers[m_Score - ((m_Score / intDivisionDigit10) * intDivisionDigit10)].Draw(dstRectfAmountOfScore);
	}
	else
	{
		//first number
		m_TextureArrayOfNumbers[m_Score / intDivisionDigit100].Draw(dstRectfAmountOfScore);
		//second number
		dstRectfAmountOfScore = { Rectf{  dstRectfScore.left + m_TextureScore.GetWidth() + m_TextureArrayOfNumbers[1].GetWidth() * 2, dstRectfScore.bottom,m_TextureArrayOfNumbers[1].GetWidth()  , m_TextureArrayOfNumbers[1].GetHeight() } };
		int temp = m_Score - (m_Score / intDivisionDigit100) * intDivisionDigit100;
		m_TextureArrayOfNumbers[temp / intDivisionDigit10].Draw(dstRectfAmountOfScore);
		//third number
		dstRectfAmountOfScore = { Rectf{  dstRectfScore.left + m_TextureScore.GetWidth() + m_TextureArrayOfNumbers[1].GetWidth() * 3, dstRectfScore.bottom,m_TextureArrayOfNumbers[1].GetWidth()  , m_TextureArrayOfNumbers[1].GetHeight() } };
		m_TextureArrayOfNumbers[temp - ((temp / intDivisionDigit10) * intDivisionDigit10)].Draw(dstRectfAmountOfScore);
	}
}

void HUD::DrawLives() const
{
	//lives
	const int nrOfRows = 3;
	Rectf dstRectLives = Rectf{ m_BottomLeftLives.x ,m_BottomLeftLives.y , m_TextureLives.GetWidth() , m_TextureLives.GetHeight() / nrOfRows };
	Rectf srcRectLives;
	srcRectLives.width = m_TextureLives.GetWidth();
	srcRectLives.height = m_TextureLives.GetHeight() / nrOfRows;
	srcRectLives.left = 0;
	srcRectLives.bottom = m_Lives * -srcRectLives.height;
	m_TextureLives.Draw(dstRectLives, srcRectLives);
}

void HUD::DrawHealthAndManaBars() const
{
	//health
	float healthBarHeight = 35;
	float healthBarXOffset = 28;
	float healthBarYOffset = 8;
	glColor4f(1.f, 0.f, 0.f, 1.f);
	utils::FillRect(Rectf{ m_BottomLeftHealthBar.x + healthBarXOffset , m_BottomLeftHealthBar.y + healthBarYOffset , float(m_Health * 2.f) , healthBarHeight });
	//bar
	Rectf dstRectHealthBar = Rectf{ m_BottomLeftHealthBar.x ,m_BottomLeftHealthBar.y , m_TextureHealthBar.GetWidth() , m_TextureHealthBar.GetHeight() };
	m_TextureHealthBar.Draw(dstRectHealthBar);
	//mana
	Rectf dstRectMana = Rectf{ dstRectHealthBar.left , dstRectHealthBar.bottom - m_TextureMana.GetHeight() , m_TextureMana.GetWidth() , m_TextureMana.GetHeight() };
	m_TextureMana.Draw(dstRectMana);
	const int offset = 3;
	glColor4f(0, 0.7f, 1, 1);
	utils::FillRect(Rectf{ dstRectMana.left + offset ,dstRectMana.bottom + offset, m_Mana , dstRectMana.height - offset * 2 });
}

void HUD::DrawAmountOfPickUps() const
{
	//amount of throwable pick ups
	const int intDivisionDigit = 10;
	Rectf dstRectfAmountOfThrowables{ Rectf{ m_BottomLeftHealthBar.x + m_TextureThrowablePickUp.GetWidth() * 1.5f , m_BottomLeftHealthBar.y - m_TextureThrowablePickUp.GetHeight() * 2.f, m_TextureThrowablePickUp.GetWidth() , m_TextureThrowablePickUp.GetHeight() } };
	if (m_AmountOfThrowables < intDivisionDigit)
	{
		m_TextureArrayOfNumbers[m_AmountOfThrowables].Draw(dstRectfAmountOfThrowables);
	}
	else
	{
		m_TextureArrayOfNumbers[m_AmountOfThrowables / intDivisionDigit].Draw(dstRectfAmountOfThrowables);
		Rectf dstRectfAmountOfThrowables = { Rectf{ m_BottomLeftHealthBar.x + m_TextureThrowablePickUp.GetWidth() + m_TextureArrayOfNumbers[1].GetWidth() * 2, m_BottomLeftHealthBar.y - m_TextureThrowablePickUp.GetHeight() * 2.f, m_TextureThrowablePickUp.GetWidth() , m_TextureThrowablePickUp.GetHeight() } };
		m_TextureArrayOfNumbers[m_AmountOfThrowables - ((m_AmountOfThrowables / intDivisionDigit) * intDivisionDigit)].Draw(dstRectfAmountOfThrowables);
	}
	//throwable pick up texture
	Rectf dstRectfThrowablePickUp = Rectf{ m_BottomLeftHealthBar.x , m_BottomLeftHealthBar.y - m_TextureThrowablePickUp.GetHeight() * 2.f, m_TextureThrowablePickUp.GetWidth() , m_TextureThrowablePickUp.GetHeight() };
	m_TextureThrowablePickUp.Draw(dstRectfThrowablePickUp);
	//health pickup texture
	Rectf dstRectfHealthPickUp = Rectf{ dstRectfAmountOfThrowables.left + m_TextureHealthPickUp.GetWidth() * 3.f, dstRectfAmountOfThrowables.bottom , m_TextureHealthPickUp.GetWidth() , m_TextureHealthPickUp.GetHeight() };
	m_TextureHealthPickUp.Draw(dstRectfHealthPickUp);
	//amount of health pick ups
	Rectf dstRectfAmountOfHealthPickUps{ Rectf{dstRectfHealthPickUp.left + dstRectfThrowablePickUp.width * 2.f , dstRectfHealthPickUp.bottom ,dstRectfHealthPickUp.width , dstRectfHealthPickUp.height} };
	m_TextureArrayOfNumbers[m_AmountOfHealthPickUps].Draw(dstRectfAmountOfHealthPickUps);
}

void HUD::DrawGoArrow() const
{
	int m_MaxCounterTime = 5;
	if (m_Counter < m_MaxCounterTime)
	{
		Rectf destRectArrow, srcRectArrow;
		int currentColumn = m_AnimGoArrowFrame % m_NrOfGoArrowFrames;
		destRectArrow = Rectf{ m_BottomLeftGoArrow.x ,m_BottomLeftGoArrow.y , m_TextureGoArrow.GetWidth() / 6 , m_TextureGoArrow.GetHeight() };
		srcRectArrow.width = m_TextureGoArrow.GetWidth() / m_NrOfGoArrowFrames;
		srcRectArrow.height = m_TextureGoArrow.GetHeight();
		srcRectArrow.left = srcRectArrow.width * currentColumn;
		srcRectArrow.bottom = 0;
		m_TextureGoArrow.Draw(destRectArrow, srcRectArrow);
	}
}

void HUD::Update(float elapsedSec, Avatar* pAvatar)
{
	m_Counter += elapsedSec;
	//
	m_Health = pAvatar->GetHealth();
	m_Mana = float(pAvatar->GetMana());
	m_Lives = pAvatar->GetLives();
	m_AmountOfThrowables = pAvatar->GetAmountOfThrowables();
	m_AmountOfHealthPickUps = pAvatar->GetAmountOfHealthPickUps();
	//
	ElapsedSecCount(elapsedSec);
}

void HUD::ElapsedSecCount(float elapsedSec)
{
	m_AnimTime += elapsedSec;
	if (m_AnimTime >= m_FrameChangeSpeed)
	{
		m_AnimGoArrowFrame++;
		if (m_AnimGoArrowFrame == m_NrOfGoArrowFrames)
		{
			m_AnimGoArrowFrame = 0;
		}
		m_AnimTime = 0;
	}
}

void HUD::SetCounterTime(float counter)
{
	m_Counter = counter;
}

void HUD::IncreaseScore(int amountOfIncrease)
{
	m_Score += amountOfIncrease;
}

void HUD::ResetScore()
{
	m_Score = 0;
}