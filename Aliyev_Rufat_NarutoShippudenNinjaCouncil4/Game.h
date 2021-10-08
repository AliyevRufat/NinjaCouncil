#pragma once
#include "Level.h"
#include "PickUpManager.h"
#include "Avatar.h"
#include "Vector2f.h"
#include "Camera.h"
#include "HUD.h"
#include "EnemyManager.h"
#include "GameStates.h"

class Game
{
public:
	explicit Game(const Window& window);
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game(Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	~Game();

	void Update(float elapsedSec);
	void Draw() const;

	// Event handling
	void ProcessKeyDownEvent(const SDL_KeyboardEvent& e);
	void ProcessKeyUpEvent(const SDL_KeyboardEvent& e);
	void ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e);
	void ProcessMouseDownEvent(const SDL_MouseButtonEvent& e);
	void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e);

private:
	// DATA MEMBERS
	Point2f m_AvatartStartPos;
	//
	const Window m_Window;
	//
	GameStates m_GameStates;
	Level m_Level;
	Avatar* m_pAvatar;
	PickUpManager m_PickUpManager;
	EnemyManager m_EnemyManager;
	Camera m_Camera;
	HUD m_Hud;
	//
	Color4f m_RestartColor;
	Point2f m_AvatarRespawnPos;
	const Point2f m_CheckPointPos;
	bool m_HasAvatarRespawned;
	bool m_IsLevel2;
	const float m_AlphaIncrease;

	// FUNCTIONS
	void Initialize();
	void Cleanup();
	void ClearBackground() const;
	void InitPickUpManager();
	void InitEnemyManagerWave1();
	void InitEnemyManagerWave2();
	void AvatarRespawn(float elapsedSec);
	void SecondLevel();
	void RestartGame(float elapsedSec);
};