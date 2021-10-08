#include "pch.h"
#include "GameStates.h"
#include <iostream>
#include "utils.h"
#include "Sound.h"

GameStates::GameStates(const Rectf& window)
	: m_AvatartStartPos{ Point2f{20,300} }
	, m_Offset{ 5 }
	, m_NrOfGameStateTextureRows{ 6 }
	, m_GameState{ GameState::startScreen }
	, m_TextureGameState{ "Resources/Images/GameStates.png" }
	, m_TextureGameOverScreen{ "Resources/Images/GameOverScreen.png" }
	, m_TextureStartScreen{ "Resources/Images/StartScreen.png" }
	, m_TextureWinScreen{ "Resources/Images/WinScreen.png" }
	, m_IsHoveringOnEffectBar{ false }
	, m_IsHoveringOnStreamBar{ false }
	, m_CanPlayGameOverMusic{ true }
	, m_PausedSongs{ false }
	, m_IsGameStarted{ false }
	, m_RestartCounter{ 0.f }
	, m_RestartCounterTime{ 3 }
	, m_RestartShape{ Rectf{0,0,0,0} }
	, m_VolumeShape{ { Rectf{window.width - window.width / 2.f - m_TextureGameState.GetWidth() / 2.f ,window.height - window.height / 2.f , m_TextureGameState.GetWidth()  , m_TextureGameState.GetHeight() / m_NrOfGameStateTextureRows  } } }
	, m_EffectVolumeSliderShape{ Rectf{window.width / 3.f * 2.f,window.height / 2.f + 22,40,70 } }
	, m_EffectBarShape{ Rectf{ window.width / 2,window.height / 2.f + 40 , window.width / 3 , 30 } }
	, m_StreamVolumeSliderShape{ Rectf{window.width / 3.f * 2.f,window.height / 2.f - 68,40,70} }
	, m_StreamBarShape{ Rectf{  window.width / 2,window.height / 2.f - 50 ,  window.width / 3  , 30 } }
	, m_WindowRectf{ window }
	, m_HalfWindowHeight{ m_WindowRectf.height / 2.f }
	, m_HalfWindowWidth{ m_WindowRectf.width / 2.f }
{
	Sound::GetInstance()->PlaySoundStream(1);
}

void GameStates::Update(float elapsedSec)
{
	const float halfTextureGameStateWidth = m_TextureGameState.GetWidth() / 2.f;
	int changeInEffectVolume;
	int changeInStreamVolume;
	const int multiplier = 100;
	switch (m_GameState)
	{
	case GameState::volume:
		Sound::GetInstance()->PauseAllSounds();
		changeInStreamVolume = int((m_StreamVolumeSliderShape.left - m_StreamBarShape.left) / (m_StreamBarShape.width - m_StreamVolumeSliderShape.width) * multiplier);
		changeInEffectVolume = int((m_EffectVolumeSliderShape.left - m_EffectBarShape.left) / (m_EffectBarShape.width - m_EffectVolumeSliderShape.width) * multiplier);
		Sound::GetInstance()->SetSoundStreamVolume(changeInStreamVolume);
		Sound::GetInstance()->SetSoundEffectVolume(changeInEffectVolume);
		break;
	case GameState::pause:
		Sound::GetInstance()->PauseAllSounds();
		m_RestartShape = { { Rectf{m_WindowRectf.width - m_WindowRectf.width / 2.f - m_TextureGameState.GetWidth() / 2.f ,m_WindowRectf.height - m_WindowRectf.height / 2.f - m_TextureGameState.GetHeight() / m_NrOfGameStateTextureRows - m_Offset, m_TextureGameState.GetWidth() , m_TextureGameState.GetHeight() / m_NrOfGameStateTextureRows  } } };
		break;
	case GameState::gameoverScreen:
		if (m_CanPlayGameOverMusic)
		{
			Sound::GetInstance()->PlaySoundStream(2);
			m_CanPlayGameOverMusic = false;
		}
		m_RestartShape = { { Rectf{m_WindowRectf.width - m_WindowRectf.width / 2.f - m_TextureGameState.GetWidth() / 2.f ,m_WindowRectf.height - m_WindowRectf.height / 2.f + m_TextureGameState.GetHeight() / m_NrOfGameStateTextureRows - m_Offset, m_TextureGameState.GetWidth() , m_TextureGameState.GetHeight() / m_NrOfGameStateTextureRows  } } };
		break;
	case GameState::winScreen:
		const int offset = 200;
		m_RestartShape = { { Rectf{m_WindowRectf.width - m_WindowRectf.width / 2.f - m_TextureGameState.GetWidth() / 2.f ,m_WindowRectf.height - m_WindowRectf.height / 2.f - offset, m_TextureGameState.GetWidth() , m_TextureGameState.GetHeight() / m_NrOfGameStateTextureRows  } } };
		break;
	}
}

void GameStates::Draw() const
{
	Rectf srcRect{};
	srcRect.width = m_TextureGameState.GetWidth();
	srcRect.height = m_TextureGameState.GetHeight() / m_NrOfGameStateTextureRows;
	srcRect.left = 0;
	//
	switch (m_GameState)
	{
	case GameState::startScreen:
		m_TextureStartScreen.Draw();
		break;
	case GameState::gameoverScreen:
		DrawGameOverSceen(srcRect);
		break;
	case GameState::pause:
		DrawPauseMenu(srcRect);
		break;
	case GameState::volume:
		DrawVolumeMenu(srcRect);
		break;
	case GameState::winScreen:
		DrawWinSceen(srcRect);
		break;
	}
	//texture on top indicating whether in pause or volume setting
	if (m_GameState == GameState::volume || m_GameState == GameState::pause)
	{
		const int xPos = 250;
		srcRect.bottom = int(m_GameState) * -srcRect.height;
		Rectf gameStateRectf = Rectf{ m_WindowRectf.width / 2.f - srcRect.width / 2.f - xPos, m_WindowRectf.height - m_WindowRectf.height / 4, srcRect.width , srcRect.height };
		m_TextureGameState.Draw(gameStateRectf, srcRect);
	}
}

void GameStates::ProcessKeyDownEvent(const SDL_KeyboardEvent& e)
{
	const int volumeChange = 10;
	//
	switch (e.keysym.sym)
	{
	case SDLK_p:
		if (m_GameState != GameState::gameoverScreen && m_GameState != GameState::startScreen && m_GameState != GameState::restart)
		{
			Sound::GetInstance()->PlaySoundEffect(16);

			if (m_GameState == GameState::pause)
			{
				m_GameState = GameState::playing;
				Sound::GetInstance()->ResumeAllSounds();
			}
			else
			{
				m_GameState = GameState::pause;
			}
		}
		break;
	case SDLK_SPACE:
		if (m_GameState == GameState::startScreen)
		{
			Sound::GetInstance()->PlaySoundEffect(16);

			m_GameState = GameState::playing;
			m_IsGameStarted = true;
			Sound::GetInstance()->ResumeAllSounds();
			Sound::GetInstance()->PlaySoundEffect(5);
		}
		break;
	case SDLK_PERIOD: // increase volume
		SoundStream::Resume();
		SoundStream::SetVolume(SoundStream::GetVolume() + volumeChange);
		Sound::GetInstance()->IncreaseSoundEffectVolume(volumeChange);
		break;
	case SDLK_COMMA: // decrease volume
		SoundStream::Resume();
		SoundStream::SetVolume(SoundStream::GetVolume() - volumeChange);
		Sound::GetInstance()->DecreaseSoundEffectVolume(volumeChange);
		break;
	case SDLK_m:
		if (m_PausedSongs)
		{
			Sound::GetInstance()->ResumeAllSounds();
			m_PausedSongs = false;
		}
		else
		{
			Sound::GetInstance()->PauseAllSounds();
			m_PausedSongs = true;
		}
		break;
	}
}

void GameStates::ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
	Point2f mousePos{ float(e.x), float(m_WindowRectf.height - e.y) };
	const int mouseOffset = 10;

	if (m_IsHoveringOnEffectBar && utils::IsOverlapping(m_EffectBarShape, m_EffectVolumeSliderShape))
	{
		m_EffectVolumeSliderShape.left = mousePos.x - mouseOffset;
	}
	else if (m_IsHoveringOnStreamBar && utils::IsOverlapping(m_StreamBarShape, m_StreamVolumeSliderShape))
	{
		m_StreamVolumeSliderShape.left = mousePos.x - mouseOffset;
	}
	//// offset adjust (limits)
	if (m_EffectVolumeSliderShape.left < m_EffectBarShape.left) // for effect slider
	{
		m_EffectVolumeSliderShape.left = m_EffectBarShape.left;
	}
	else if (m_EffectVolumeSliderShape.left > m_EffectBarShape.left + m_EffectBarShape.width - m_EffectVolumeSliderShape.width)
	{
		m_EffectVolumeSliderShape.left = m_EffectBarShape.left + m_EffectBarShape.width - m_EffectVolumeSliderShape.width;
	}
	if (m_StreamVolumeSliderShape.left < m_StreamBarShape.left) // for Stream slider
	{
		m_StreamVolumeSliderShape.left = m_StreamBarShape.left;
	}
	else if (m_StreamVolumeSliderShape.left > m_StreamBarShape.left + m_StreamBarShape.width - m_StreamVolumeSliderShape.width)
	{
		m_StreamVolumeSliderShape.left = m_StreamBarShape.left + m_StreamBarShape.width - m_StreamVolumeSliderShape.width;
	}
}

void GameStates::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e)
{
	Point2f mousePos{ float(e.x), float(m_WindowRectf.height - e.y) };

	if (utils::IsPointInRect(mousePos, m_RestartShape) && (m_GameState == GameState::pause || m_GameState == GameState::gameoverScreen || m_GameState == GameState::winScreen))
	{
		Sound::GetInstance()->PlaySoundEffect(16);
		Sound::GetInstance()->ResumeAllSounds();
		m_CanPlayGameOverMusic = true;
		m_GameState = GameState::restart;
	}
	else if (utils::IsPointInRect(mousePos, m_VolumeShape) && m_GameState == GameState::pause)
	{
		Sound::GetInstance()->PlaySoundEffect(16);

		m_GameState = GameState::volume;
	}
	//
	if (m_GameState == GameState::volume)
	{
		if (utils::IsPointInRect(mousePos, m_EffectVolumeSliderShape))
		{
			m_IsHoveringOnEffectBar = true;
		}
		else if (utils::IsPointInRect(mousePos, m_StreamVolumeSliderShape))
		{
			m_IsHoveringOnStreamBar = true;
		}
	}
}

void GameStates::ProcessMouseUpEvent(const SDL_MouseButtonEvent& e)
{
	m_IsHoveringOnEffectBar = false;
	m_IsHoveringOnStreamBar = false;
}

void GameStates::DrawGameOverSceen(Rectf& srcRect) const
{
	glPushMatrix();
	glScalef(1.5f, 1.5f, 1);
	const float xPos = m_WindowRectf.width / 20.f;
	m_TextureGameOverScreen.Draw(Rectf{ xPos ,0 ,0 ,0 });
	glScalef(0.666f, 0.666f, 1);
	glPopMatrix();
	const int restartColumn = 2;
	//restart texture
	srcRect.bottom = restartColumn * -srcRect.height;
	m_TextureGameState.Draw(m_RestartShape, srcRect);
}

void GameStates::DrawPauseMenu(Rectf& srcRect) const
{
	glColor4f(0.f, 0.f, 0.f, 0.5f);
	utils::FillRect(Rectf{ 0,0,m_WindowRectf.width,m_WindowRectf.height });
	//volume texture
	const int volumeColumn = 4;
	const int restartColumn = 2;

	srcRect.bottom = volumeColumn * -srcRect.height;
	m_TextureGameState.Draw(m_VolumeShape, srcRect);
	//restart texture
	srcRect.bottom = restartColumn * -srcRect.height;
	m_TextureGameState.Draw(m_RestartShape, srcRect);
}

void GameStates::DrawVolumeMenu(Rectf& srcRect) const
{
	//alpha decrease
	glColor4f(0.f, 0.f, 0.f, 0.5f);
	utils::FillRect(Rectf{ 0,0,m_WindowRectf.width,m_WindowRectf.height });
	//
	const int effectColumn = 1;
	const int StreamColumn = 0;
	const int divider = 10;
	//effects texture
	srcRect.bottom = effectColumn * -srcRect.height;
	Rectf effectsAndStreamRectf = { Rectf{m_WindowRectf.width / divider,m_WindowRectf.height / 2.f + m_TextureGameState.GetHeight() / divider / 2,m_TextureGameState.GetWidth(),m_TextureGameState.GetHeight() / divider} };
	m_TextureGameState.Draw(effectsAndStreamRectf, srcRect);

	//Stream texture
	srcRect.bottom = StreamColumn * -srcRect.height;
	effectsAndStreamRectf = { Rectf{m_WindowRectf.width / divider,m_WindowRectf.height / 2.f - m_TextureGameState.GetHeight() / divider, m_TextureGameState.GetWidth(),m_TextureGameState.GetHeight() / divider} };
	m_TextureGameState.Draw(effectsAndStreamRectf, srcRect);

	//effect and Stream bars
	glColor4f(0.8f, 0.8f, 0.f, 1.f);
	utils::FillRect(m_EffectBarShape); //effect bar
	utils::FillRect(m_StreamBarShape); //Stream bar

	//sliders to change the volume
	glColor4f(0.9f, 0.f, 0.f, 1.f);
	utils::FillRect(m_EffectVolumeSliderShape); //effect bar
	utils::FillRect(m_StreamVolumeSliderShape); //Stream bar
}

void GameStates::DrawWinSceen(Rectf& srcRect) const
{
	glColor4f(0.f, 0.f, 0.f, 0.65f);
	utils::FillRect(Rectf{ 0,0,m_WindowRectf.width,m_WindowRectf.height });
	//
	glPushMatrix();
	glScalef(1.5f, 1.5f, 1);
	m_TextureWinScreen.Draw(Rectf{ m_WindowRectf.width / 20.f  ,0 ,0 ,0 });
	glScalef(0.666f, 0.666f, 1);
	glPopMatrix();
	const int restartColumn = 2;
	//restart texture
	srcRect.bottom = restartColumn * -srcRect.height;
	m_TextureGameState.Draw(m_RestartShape, srcRect);
}

GameStates::GameState GameStates::GetGameState() const
{
	return m_GameState;
}

void GameStates::SetGameState(GameState gameState)
{
	m_GameState = gameState;
}

bool GameStates::GetIsGameStarted()const
{
	return m_IsGameStarted;
}

void GameStates::SetIsGameStarted(bool isGameStarted)
{
	m_IsGameStarted = isGameStarted;
}