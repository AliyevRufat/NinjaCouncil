#include "pch.h"
#include <iostream>
#include "Game.h"
#include "utils.h"
#include "Sound.h"

Game::Game(const Window& window)
	:m_Window{ window }
	, m_AvatartStartPos{ Point2f{20,300} }
	, m_Camera{ m_Window.width,m_Window.height }
	, m_pAvatar{ new Avatar(m_AvatartStartPos) }
	, m_RestartColor{ Color4f{0,0,0,0} }
	, m_Hud{ Rectf{0,0,window.width , window.height} }
	, m_AvatarRespawnPos(Point2f{ 20,300 })
	, m_CheckPointPos{ Point2f{ 5300,1070 } }
	, m_HasAvatarRespawned{ false }
	, m_IsLevel2{ false }
	, m_AlphaIncrease{ 0.8f }
	, m_GameStates{ Rectf{0,0,window.width , window.height} }
{
	Sound::GetInstance()->PlaySoundStream(1);
}

Game::~Game()
{
	Cleanup();
}

void Game::Initialize()
{
	m_Camera.SetLevelBoundaries(m_Level.GetBoundaries());
	InitPickUpManager();
	InitEnemyManagerWave1();
	Sound::GetInstance()->PlaySoundStream(0);
}

void Game::Cleanup()
{
	delete m_pAvatar;
	delete Sound::GetInstance();
}

void Game::Update(float elapsedSec)
{
	//win
	const int winPosY = 150;
	const int xOffset = 150;
	if (m_GameStates.GetGameState() == GameStates::GameState::playing && m_pAvatar->GetShape().left > m_Level.GetBoundaries().width * 2.f - xOffset && m_pAvatar->GetShape().bottom < winPosY && m_EnemyManager.AreEnemiesDead())
	{
		m_GameStates.SetGameState(GameStates::GameState::winScreen);
		const Point2f drawPos = Point2f{ 350,200 };
	}
	//game started , spawn everything
	if (m_GameStates.GetIsGameStarted())
	{
		Initialize();
		m_GameStates.SetIsGameStarted(false);
	}
	//set checkpoint if a pos is reached
	if (m_pAvatar->GetShape().left > m_CheckPointPos.x&& m_pAvatar->GetShape().bottom > m_CheckPointPos.y)
	{
		m_AvatarRespawnPos = m_CheckPointPos;
	}
	//gamestates
	if (m_GameStates.GetGameState() == GameStates::GameState::restart)
	{
		RestartGame(elapsedSec);
	}
	else if (m_GameStates.GetGameState() == GameStates::GameState::playing)
	{
		m_pAvatar->Update(elapsedSec, m_Level);
		m_PickUpManager.Update(elapsedSec, m_pAvatar, m_Level, m_Hud);
		m_Camera.Update(elapsedSec, m_pAvatar);
		m_Level.Update(elapsedSec, m_pAvatar);
		m_EnemyManager.Update(elapsedSec, m_Level, m_pAvatar, m_PickUpManager, m_Hud);
		m_Hud.Update(elapsedSec, m_pAvatar);
		//chekpoint
		AvatarRespawn(elapsedSec);
		//
		if (m_EnemyManager.AreEnemiesDead() && !m_IsLevel2)
		{
			SecondLevel();
			m_IsLevel2 = true;
		}
	}
	//
	m_GameStates.Update(elapsedSec);
}

void Game::Draw() const
{
	ClearBackground();
	//
	if (m_GameStates.GetGameState() != GameStates::GameState::startScreen)
	{
		//parallax background
		glPushMatrix();
		glScalef(2, 2, 1);
		m_Camera.TransformBackGround(m_pAvatar->GetShape());
		m_Level.DrawBackground();
		glScalef(0.5, 0.5, 1);
		glPopMatrix();
		//foreground
		glPushMatrix();
		glScalef(2, 2, 1);
		m_Camera.TransformForeGround(m_pAvatar->GetShape());
		glScalef(0.5, 0.5, 1);
		m_Level.DrawForeground();
		glScalef(2, 2, 1);
		m_PickUpManager.Draw();
		m_EnemyManager.Draw();
		m_pAvatar->Draw();
		glScalef(0.5, 0.5, 1);
		glPopMatrix();
		//
		m_Hud.Draw();
		//alpha decrease when dead
		glColor4f(m_RestartColor.r, m_RestartColor.g, m_RestartColor.b, m_RestartColor.a);
		utils::FillRect(Rectf{ 0,0,m_Window.width , m_Window.height });
	}
	//
	m_GameStates.Draw();
}

void Game::ProcessKeyDownEvent(const SDL_KeyboardEvent& e)
{
	m_GameStates.ProcessKeyDownEvent(e);
	//
	m_pAvatar->ProcessKeyDownEvent(e);
	//
	switch (e.keysym.sym)
	{
	case SDLK_i:
		std::cout << "Use the Left and Right arrows to move." << '\n' << "Use the Up arrow to jump." << '\n' << "Press A to attack." << '\n' << "Press Q to throw a weapon." << '\n' << "Press S for special attack if mana is full (top left blue bar)." << '\n' << "Press H to heal if you have a Health Pick Up." << '\n' << "Use comma and period keys to decrease and increase the game volume. And m key to mute/unmute the sounds." << '\n' << "Press SPACE to start the game." << '\n' << "Press ESCAPE to close the game." << '\n';
		break;
	case SDLK_ESCAPE:
		// to quit
		SDL_Event sdlEvent;
		sdlEvent.type = SDL_QUIT;
		SDL_PushEvent(&sdlEvent);
		break;
	}
}

void Game::ProcessKeyUpEvent(const SDL_KeyboardEvent& e)
{
}

void Game::ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
	m_GameStates.ProcessMouseMotionEvent(e);
}

void Game::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e)
{
	m_GameStates.ProcessMouseDownEvent(e);
}

void Game::ProcessMouseUpEvent(const SDL_MouseButtonEvent& e)
{
	m_GameStates.ProcessMouseUpEvent(e);
}

void Game::ClearBackground() const
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Game::AvatarRespawn(float elapsedSec)
{
	if (m_pAvatar->IsDead() && !m_HasAvatarRespawned)
	{
		m_RestartColor.a += m_AlphaIncrease * elapsedSec;
		if (m_RestartColor.a >= 1)
		{
			m_RestartColor.a = 1;

			if (m_pAvatar->GetLives() == 0 && m_pAvatar->GetHealth() <= 0)
			{
				m_GameStates.SetGameState(GameStates::GameState::gameoverScreen);
				return;
			}
			m_pAvatar->SetPos(m_AvatarRespawnPos);
			m_pAvatar->Respawn();
			m_HasAvatarRespawned = true;
		}
	}
	else if (m_HasAvatarRespawned && m_GameStates.GetGameState() != GameStates::GameState::gameoverScreen)
	{
		m_RestartColor.a -= m_AlphaIncrease * elapsedSec;
		if (m_RestartColor.a <= 0)
		{
			m_RestartColor.a = 0;
			m_HasAvatarRespawned = false;
		}
	}
}

void Game::SecondLevel()
{
	m_Camera.SetHasMovedToNewLevel(true);
	m_Hud.SetCounterTime(0.0f);
	InitEnemyManagerWave2();
}

void Game::RestartGame(float elapsedSec)
{
	m_Camera.SetHasMovedToNewLevel(true);
	//
	if (!m_HasAvatarRespawned)
	{
		m_RestartColor.a += m_AlphaIncrease * elapsedSec;
		if (m_RestartColor.a >= 1)
		{
			m_RestartColor.a = 1;
			m_HasAvatarRespawned = true;
		}
	}
	else if (m_HasAvatarRespawned)
	{
		m_RestartColor.a = 0;
		m_pAvatar->SetPos(m_AvatartStartPos);
		m_pAvatar->Restart();
		m_EnemyManager.DeleteAllEnemies();
		m_PickUpManager.DeleteAllPickUps();
		m_Hud.ResetScore();
		Initialize();
		Sound::GetInstance()->PlaySoundEffect(5);
		m_GameStates.SetGameState(GameStates::GameState::playing);
		m_HasAvatarRespawned = false;
		m_IsLevel2 = false;
	}
}

void Game::InitPickUpManager()
{
	/////////////////////level 1
	m_PickUpManager.AddItem(Point2f{ 100,260 }, PickUp::Type::health);
	m_PickUpManager.AddItem(Point2f{ 130,260 }, PickUp::Type::weapon);
	m_PickUpManager.AddItem(Point2f{ 750,92 }, PickUp::Type::weapon);
	m_PickUpManager.AddItem(Point2f{ 1700,470 }, PickUp::Type::weapon);
	m_PickUpManager.AddItem(Point2f{ 645,465 }, PickUp::Type::health);
	m_PickUpManager.AddItem(Point2f{ 2280,450 }, PickUp::Type::health);
	m_PickUpManager.AddItem(Point2f{ 3750,450 }, PickUp::Type::weapon);
	m_PickUpManager.AddItem(Point2f{ 3780,450 }, PickUp::Type::weapon);
	m_PickUpManager.AddItem(Point2f{ 2780,770 }, PickUp::Type::health);
	m_PickUpManager.AddItem(Point2f{ 1730,470 }, PickUp::Type::coin);
	m_PickUpManager.AddItem(Point2f{ 2320,450 }, PickUp::Type::coin);
	m_PickUpManager.AddItem(Point2f{ 2810,770 }, PickUp::Type::coin);
	/////////////////////level 2
	m_PickUpManager.AddItem(Point2f{ 7000,90 }, PickUp::Type::health);
	m_PickUpManager.AddItem(Point2f{ 7070,90 }, PickUp::Type::weapon);
	m_PickUpManager.AddItem(Point2f{ 7270,90 }, PickUp::Type::weapon);
	m_PickUpManager.AddItem(Point2f{ 6166,1075 }, PickUp::Type::weapon);
	m_PickUpManager.AddItem(Point2f{ 6240,1075 }, PickUp::Type::health);
	m_PickUpManager.AddItem(Point2f{ 7700,445 }, PickUp::Type::weapon);
	m_PickUpManager.AddItem(Point2f{ 7730,445 }, PickUp::Type::weapon);
	m_PickUpManager.AddItem(Point2f{ 7760,445 }, PickUp::Type::weapon);
	m_PickUpManager.AddItem(Point2f{ 7790,445 }, PickUp::Type::weapon);
	m_PickUpManager.AddItem(Point2f{ 7820,445 }, PickUp::Type::weapon);
	m_PickUpManager.AddItem(Point2f{ 9700,445 }, PickUp::Type::coin);
	m_PickUpManager.AddItem(Point2f{ 9730,445 }, PickUp::Type::coin);
	m_PickUpManager.AddItem(Point2f{ 9760,445 }, PickUp::Type::coin);
	m_PickUpManager.AddItem(Point2f{ 9790,445 }, PickUp::Type::coin);
	m_PickUpManager.AddItem(Point2f{ 9820,445 }, PickUp::Type::coin);
	m_PickUpManager.AddItem(Point2f{ 9560,1070 }, PickUp::Type::coin);
	m_PickUpManager.AddItem(Point2f{ 9590,1070 }, PickUp::Type::coin);
	m_PickUpManager.AddItem(Point2f{ 9620,1070 }, PickUp::Type::coin);
	m_PickUpManager.AddItem(Point2f{ 9650,1070 }, PickUp::Type::coin);
	m_PickUpManager.AddItem(Point2f{ 9680,1070 }, PickUp::Type::coin);
	m_PickUpManager.AddItem(Point2f{ 9710,1070 }, PickUp::Type::coin);
	m_PickUpManager.AddItem(Point2f{ 9740,1070 }, PickUp::Type::coin);
	m_PickUpManager.AddItem(Point2f{ 9770,1070 }, PickUp::Type::coin);
	m_PickUpManager.AddItem(Point2f{ 9800,1070 }, PickUp::Type::coin);
	m_PickUpManager.AddItem(Point2f{ 9830,1070 }, PickUp::Type::coin);
}

void Game::InitEnemyManagerWave1()
{
	m_EnemyManager.AddItem(Point2f{ 470,92 }, EnemyManager::TypeEnemy::rangeEnemy);
	m_EnemyManager.AddItem(Point2f{ 1650,90 }, EnemyManager::TypeEnemy::meleeEnemy);
	m_EnemyManager.AddItem(Point2f{ 1850,470 }, EnemyManager::TypeEnemy::rangeEnemy);
	m_EnemyManager.AddItem(Point2f{ 1300,470 }, EnemyManager::TypeEnemy::rangeEnemy);
	m_EnemyManager.AddItem(Point2f{ 625,465 }, EnemyManager::TypeEnemy::rangeEnemy);
	m_EnemyManager.AddItem(Point2f{ 2665,450 }, EnemyManager::TypeEnemy::rangeEnemy);
	m_EnemyManager.AddItem(Point2f{ 3400,450 }, EnemyManager::TypeEnemy::meleeBoss);
	m_EnemyManager.AddItem(Point2f{ 2885,770 }, EnemyManager::TypeEnemy::meleeEnemy);
	m_EnemyManager.AddItem(Point2f{ 1400,770 }, EnemyManager::TypeEnemy::rangeEnemy);
	m_EnemyManager.AddItem(Point2f{ 1450,770 }, EnemyManager::TypeEnemy::meleeEnemy);
	m_EnemyManager.AddItem(Point2f{ 1000,1130 }, EnemyManager::TypeEnemy::rangeEnemy);
	m_EnemyManager.AddItem(Point2f{ 3960,1080 }, EnemyManager::TypeEnemy::rangeEnemy);
	m_EnemyManager.AddItem(Point2f{ 4800,1080 }, EnemyManager::TypeEnemy::meleeBoss);
}

void Game::InitEnemyManagerWave2()
{
	m_EnemyManager.AddItem(Point2f{ 6605,770 }, EnemyManager::TypeEnemy::rangeEnemy);
	m_EnemyManager.AddItem(Point2f{ 5550,90 }, EnemyManager::TypeEnemy::meleeEnemy);
	m_EnemyManager.AddItem(Point2f{ 6090,90 }, EnemyManager::TypeEnemy::meleeBoss);
	m_EnemyManager.AddItem(Point2f{ 7520,90 }, EnemyManager::TypeEnemy::rangeEnemy);
	m_EnemyManager.AddItem(Point2f{ 8770,90 }, EnemyManager::TypeEnemy::rangeBoss);
	m_EnemyManager.AddItem(Point2f{ 8250,90 }, EnemyManager::TypeEnemy::meleeEnemy);
	m_EnemyManager.AddItem(Point2f{ 5480,770 }, EnemyManager::TypeEnemy::rangeEnemy);
	m_EnemyManager.AddItem(Point2f{ 5800,1075 }, EnemyManager::TypeEnemy::meleeEnemy);
	m_EnemyManager.AddItem(Point2f{ 5850,1075 }, EnemyManager::TypeEnemy::rangeEnemy);
	m_EnemyManager.AddItem(Point2f{ 8700,445 }, EnemyManager::TypeEnemy::rangeEnemy);
	m_EnemyManager.AddItem(Point2f{ 8500,445 }, EnemyManager::TypeEnemy::meleeEnemy);
	m_EnemyManager.AddItem(Point2f{ 9600,445 }, EnemyManager::TypeEnemy::meleeBoss);
	m_EnemyManager.AddItem(Point2f{ 9150,770 }, EnemyManager::TypeEnemy::rangeEnemy);
	m_EnemyManager.AddItem(Point2f{ 9700,770 }, EnemyManager::TypeEnemy::meleeEnemy);
	m_EnemyManager.AddItem(Point2f{ 7200,770 }, EnemyManager::TypeEnemy::meleeEnemy);
}