#include "pch.h"
#include "Game.h"
#include "iostream"
#include "Bones.h"
#include "Bomb.h"
#include "utils.h"
#include "SpecialBomb.h"
//using namespace utils;

Game::Game( const Window& window ) 
	:BaseGame{ window }
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	m_Position.x = GetViewPort().width / 2;
	m_Position.y = GetViewPort().height / 2;

	m_IsDead = false;
	m_Timer = 0;
	m_Score = 0;
	m_GameTimer = 60;
	m_GameTimerPrecise = 0.f;
	m_TimeEnded = false;

	m_pText = new Texture("Score: " + std::to_string(m_Score), "DIN-Light.otf", 40.f, Color4f{ 1.f, 1.f, 1.f, 1.f });
	m_pGameOver = new Texture("GAME OVER", "DIN-Light.otf", 150.f, Color4f{ 0.f, 0.f, 0.f, 1.f });
	m_pGameOver2 = new Texture("Press R to restart", "DIN-Light.otf", 40.f, Color4f{ 0.f, 0.f, 0.f, 1.f });
	m_pTimer = new Texture("Time left: " + std::to_string(m_GameTimer), "DIN-Light.otf", 40.f, Color4f{ 1.f, 1.f, 1.f, 1.f });
	m_pTimeEnded = new Texture("TIME ENDED", "DIN-Light.otf", 140.f, Color4f{ 0.f, 0.f, 0.f, 1.f });
	m_pInfoText = new Texture("Use arrows to move, press space to catch a bone( stops speed)", "DIN-Light.otf", 20.f, Color4f{ 1.f, 1.f, 1.f, 1.f });
}

void Game::Cleanup( )
{
	for (int idx{}; idx < m_Bones.size(); ++idx)
	{
		delete m_Bones[idx];
		m_Bones[idx] = nullptr;
	}
	for (int idx{}; idx < m_Bombs.size(); ++idx)
	{
		delete m_Bombs[idx];
		m_Bombs[idx] = nullptr;
	}
	for (int idx{}; idx < m_SpecialBombs.size(); ++idx)
	{
		delete m_SpecialBombs[idx];
		m_SpecialBombs[idx] = nullptr;
	}

	delete m_pText;
	m_pText = nullptr;
	delete m_pGameOver;
	m_pGameOver = nullptr;
	delete m_pGameOver2;
	m_pGameOver2 = nullptr;
	delete m_pTimer;
	m_pTimer = nullptr;
	delete m_pTimeEnded;
	m_pTimeEnded = nullptr;
	delete m_pInfoText;
	m_pInfoText = nullptr;
}

void Game::Update( float elapsedSec )
{	
	if (!m_IsDead)
	{
		m_Position.x += m_Velocity.x * elapsedSec;
		m_Position.y += m_Velocity.y * elapsedSec;

		m_Timer += elapsedSec;
		m_GameTimerPrecise += elapsedSec;

		if (m_GameTimerPrecise >= 1.f)
		{
			--m_GameTimer;
			delete m_pTimer;
			m_pTimer = nullptr;
			m_pTimer = new Texture("Time left: " + std::to_string(m_GameTimer), "DIN-Light.otf", 40.f, Color4f{1.f, 1.f, 1.f, 1.f});
			m_GameTimerPrecise = 0.f;
		}

		if (m_GameTimer >= 50 && m_GameTimer < 60 && m_Timer >= 1.5f)
		{
			CreateObject();
			m_Timer = 0;
		}
		else if (m_GameTimer >= 40 && m_GameTimer < 50 && m_Timer >= 0.5f)
		{
			CreateObject();
			CreateSpecialBombs(20);
			m_Timer = 0;
		}
		else if (m_GameTimer >= 25 && m_GameTimer < 40 && m_Timer >= 0.2f)
		{
			CreateObject();
			CreateSpecialBombs(7);
			m_Timer = 0;
		}
		else if (m_GameTimer < 25 && m_Timer >= 0.05f)
		{
			CreateObject();
			CreateSpecialBombs(15);
			m_Timer = 0;
		}
		else if (m_GameTimer <= 0)
		{
			m_TimeEnded = true;
			m_GameTimer = 60;
			m_HighScores.push_back(m_Score);
			m_IsDead = true;

			std::cout << "-- HIGHSCORES --\n";
			for (int idx{}; idx < m_HighScores.size(); ++idx)
			{
				std::cout << "Score " << idx + 1 << ": " << m_HighScores[idx] << "\n";
			}
			std::cout << std::endl;
		}

		m_Hitbox = Rectf{ m_Position.x, m_Position.y, 25.f, 50.f, };

		for (int idx{}; idx < m_Bones.size(); ++idx)
		{
			if (m_Bones[idx] != nullptr)
			{
				m_Bones[idx]->CollisionDetection(m_Hitbox);
				m_Bones[idx]->Update(elapsedSec);
				if (m_Bones[idx]->IsDestroyed())
				{
					delete m_Bones[idx];
					m_Bones[idx] = nullptr;
				}
			}
		}

		for (int idx{}; idx < m_Bombs.size(); ++idx)
		{
			if (m_Bombs[idx] != nullptr)
			{
				m_Bombs[idx]->CollisionDetection(m_Hitbox);
				if (m_Bombs[idx]->CollisionDetection(m_Hitbox))
				{
					m_IsDead = true;
					m_HighScores.push_back(m_Score);
					std::cout << "-- HIGHSCORES --\n";
					for (int idx{}; idx < m_HighScores.size(); ++idx)
					{
						std::cout << "Score " << idx + 1 << ": " << m_HighScores[idx] << "\n";
					}
					std::cout << std::endl;
				}
				m_Bombs[idx]->Update(elapsedSec);
				if (m_Bombs[idx]->IsDestroyed())
				{
					delete m_Bombs[idx];
					m_Bombs[idx] = nullptr;
				}
			}
		}

		for (int idx{}; idx < m_SpecialBombs.size(); ++idx)
		{
			if (m_SpecialBombs[idx] != nullptr)
			{
				m_SpecialBombs[idx]->CollisionDetection(m_Hitbox);
				if (m_SpecialBombs[idx]->CollisionDetection(m_Hitbox))
				{
					m_IsDead = true;
					m_HighScores.push_back(m_Score);
					std::cout << "-- HIGHSCORES --\n";
					for (int idx{}; idx < m_HighScores.size(); ++idx)
					{
						std::cout << "Score " << idx + 1 << ": " << m_HighScores[idx] << "\n";
					}
					std::cout << std::endl;
				}
				m_SpecialBombs[idx]->Update(elapsedSec);
				if (m_SpecialBombs[idx]->IsDestroyed())
				{
					delete m_SpecialBombs[idx];
					m_SpecialBombs[idx] = nullptr;
				}
			}
		}


		if (m_Position.x < 0 )
		{
			m_Position.x = GetViewPort().width;
		}
		if (m_Position.x > GetViewPort().width)
		{
			m_Position.x = 0;
		}
		if (m_Position.y < 0)
		{
			m_Position.y = GetViewPort().height;
		}
		if (m_Position.y > GetViewPort().height)
		{
			m_Position.y = 0;
		}
	}		
}

void Game::Draw( ) const
{
	ClearBackground();	

	m_pTimer->Draw(Point2f{ 50.f, GetViewPort().height - 45.f });
	m_pText->Draw(Point2f{ GetViewPort().width / 2 - 55.f, GetViewPort().height - 45.f });
	m_pInfoText->Draw(Point2f{ GetViewPort().width / 2 - 250.f, GetViewPort().height - 60.f });

	for (int idx{}; idx < m_Bones.size(); ++idx)
	{
		if (m_Bones[idx] != nullptr)
		{
			m_Bones[idx]->Draw();
		}
	}

	for (int idx{}; idx < m_Bombs.size(); ++idx)
	{
		if (m_Bombs[idx] != nullptr)
		{
			m_Bombs[idx]->Draw();
		}
	}
	
	for (int idx{}; idx < m_SpecialBombs.size(); ++idx)
	{
		if (m_SpecialBombs[idx] != nullptr)
		{
			m_SpecialBombs[idx]->Draw();
		}
	}

	if (!m_IsDead)
	{

		utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });

		//DrawRect(m_Position, 25.f, 50.f, 2.f);
		utils::FillEllipse(Point2f{ m_Position.x + 12.5f, m_Position.y + 40.f }, 5.f, 5.f);
		utils::DrawLine(Point2f{ m_Position.x + 12.5f, m_Position.y + 40.f }, Point2f{ m_Position.x + 12.5f, m_Position.y + 20.f }, 2.f);
		utils::DrawLine(Point2f{ m_Position.x + 5.f, m_Position.y + 6.f }, Point2f{ m_Position.x + 12.5f, m_Position.y + 20.f }, 2.f);
		utils::DrawLine(Point2f{ m_Position.x + 20.f, m_Position.y + 6.f }, Point2f{ m_Position.x + 12.5f, m_Position.y + 20.f }, 2.f);
		utils::DrawLine(Point2f{ m_Position.x + 5.f, m_Position.y + 25.f }, Point2f{ m_Position.x + 12.5f, m_Position.y + 35.f }, 2.f);
		utils::DrawLine(Point2f{ m_Position.x + 20.f, m_Position.y + 25.f }, Point2f{ m_Position.x + 12.5f, m_Position.y + 35.f }, 2.f);
	}
	else if (!m_TimeEnded)
	{
		m_pGameOver->Draw(Point2f{ GetViewPort().width / 2 - 400.f, GetViewPort().height / 2 - 100.f });
		m_pGameOver2->Draw(Point2f{ GetViewPort().width / 2 - 150.f, GetViewPort().height / 2 - 150.f });
	}

	if (m_TimeEnded)
	{
		m_pTimeEnded->Draw(Point2f{ GetViewPort().width / 2 - 400.f, GetViewPort().height / 2 - 100.f });
		m_pGameOver2->Draw(Point2f{ GetViewPort().width / 2 - 150.f, GetViewPort().height / 2 - 150.f });
	}
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	if (!m_IsDead)
	{
		const float speed{ 200.f };
		if (e.keysym.sym == SDLK_LEFT)
		{
			m_Velocity.x = -speed;
		}
		else if (e.keysym.sym == SDLK_RIGHT)
		{
			m_Velocity.x = speed;
		}
		else m_Velocity.x = 0.f;

		if (e.keysym.sym == SDLK_DOWN)
		{
			m_Velocity.y = -speed;
		}
		else if (e.keysym.sym == SDLK_UP)
		{
			m_Velocity.y = speed;
		}
		else m_Velocity.y = 0.f;

		if (e.keysym.sym == SDLK_i)
		{
			std::cout << m_Score << "\n";
		}

		if (e.keysym.sym == SDLK_SPACE)
		{
			for (int idx{}; idx < m_Bones.size(); ++idx)
			{
				if (m_Bones[idx] != nullptr)
				{
					if (m_Bones[idx]->CollisionDetection(m_Hitbox))
					{
						++m_Score;
						delete m_pText;
						m_pText = nullptr;
						m_pText = new Texture("Score: " + std::to_string(m_Score), "DIN-Light.otf", 40.f, Color4f{ 1.f, 1.f, 1.f, 1.f });
						m_Bones[idx]->DestroyBone();
					}
				}				
			}
		}
	}	
	if (m_IsDead)
	{
		if (m_GameTimer != 60)
		{
			m_GameTimer = 60;
			delete m_pTimer;
			m_pTimer = nullptr;
			m_pTimer = new Texture("Time left: " + std::to_string(m_GameTimer), "DIN-Light.otf", 40.f, Color4f{ 1.f, 1.f, 1.f, 1.f });

		}
		if (e.keysym.sym == SDLK_r)
		{
			ResetGame();
		}
	}
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
}

void Game::ClearBackground( ) const
{
	glClearColor(0.6f, 0.5f, 0.5f, 1.0f);
	glClear( GL_COLOR_BUFFER_BIT );
}

void Game::CreateObject()
{
	int random{ rand() % 2 };
	if (random == 0) 
	{
		m_Bones.push_back(new Bones(GetViewPort().width, GetViewPort().height));
		//std::cout << "New Bone created!" << std::endl;
	}
	else 
	{
		m_Bombs.push_back(new Bomb(GetViewPort().width, GetViewPort().height));
		//std::cout << "New Bomb created!" << std::endl;
		//m_SpecialBombs.push_back(new SpecialBomb(GetViewPort().width, GetViewPort().height));
	}
}

void Game::CreateSpecialBombs(int spawnChance)
{
	int random{ rand() % spawnChance };
	if (random == spawnChance / 2)
	{
		m_SpecialBombs.push_back(new SpecialBomb(GetViewPort().width, GetViewPort().height));
	}
}

void Game::ResetGame()
{
	for (int idx{}; idx < m_Bones.size(); ++idx)
	{
		delete m_Bones[idx];
		m_Bones[idx] = nullptr;
	}

	for (int idx{}; idx < m_Bombs.size(); ++idx)
	{
		delete m_Bombs[idx];
		m_Bombs[idx] = nullptr;
	}

	for (int idx{}; idx < m_SpecialBombs.size(); ++idx)
	{
		delete m_SpecialBombs[idx];
		m_SpecialBombs[idx] = nullptr;
	}

	m_Score = 0;
	m_Timer = 0;
	m_IsDead = false;
	m_GameTimer = 60.f;
	m_GameTimerPrecise = 0.f;

	delete m_pText;
	m_pText = nullptr;
	m_pText = new Texture("Score: " + std::to_string(m_Score), "DIN-Light.otf", 40.f, Color4f{ 1.f, 1.f, 1.f, 1.f });
}
