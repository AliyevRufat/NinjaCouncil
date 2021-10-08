#pragma once
#include "Level.h"
#include "PickUpManager.h"
#include "Vector2f.h"
#include "Camera.h"
#include "HUD.h"
#include "EnemyManager.h"

class GameStates
{
public:
	enum class GameState
	{
		restart = 0,
		playing = 1,
		startScreen = 2,
		gameoverScreen = 3,
		volume = 4,
		pause = 5,
		winScreen = 6
	};

	GameStates(const Rectf& window);
	void Update(float elapsedSec);
	void Draw() const;
	// Event handling
	void ProcessKeyDownEvent(const SDL_KeyboardEvent& e);
	void ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e);
	void ProcessMouseDownEvent(const SDL_MouseButtonEvent& e);
	void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e);
	//
	GameState GetGameState() const;
	void SetGameState(GameState gameState);
	//
	bool GetIsGameStarted() const;
	void SetIsGameStarted(bool isGameStarted);
	//
private:
	// DATA MEMBERS
	Point2f m_AvatartStartPos;
	//
	const int m_Offset;
	const int m_NrOfGameStateTextureRows;
	const float m_HalfWindowHeight;
	const float m_HalfWindowWidth;
	//
	GameState m_GameState;
	//
	Texture m_TextureGameState;
	Texture m_TextureGameOverScreen;
	Texture m_TextureStartScreen;
	Texture m_TextureWinScreen;
	//
	bool m_IsHoveringOnEffectBar;
	bool m_IsHoveringOnStreamBar;
	bool m_CanPlayGameOverMusic;
	bool m_PausedSongs;
	bool m_IsGameStarted;
	//
	float m_RestartCounter;
	const int m_RestartCounterTime;
	//
	Rectf m_RestartShape;
	Rectf m_VolumeShape;
	//
	Rectf m_EffectVolumeSliderShape;
	Rectf m_EffectBarShape;
	//
	Rectf m_StreamVolumeSliderShape;
	Rectf m_StreamBarShape;
	//
	Rectf m_WindowRectf;
	// METHODS
	void DrawVolumeMenu(Rectf& srcRect) const;
	void DrawPauseMenu(Rectf& srcRect) const;
	void DrawGameOverSceen(Rectf& srcRect)	const;
	void DrawWinSceen(Rectf& srcRect)	const;
};
