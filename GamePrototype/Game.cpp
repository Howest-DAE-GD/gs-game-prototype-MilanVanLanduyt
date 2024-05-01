#include "pch.h"
#include "Game.h"
#include "iostream"
#include "Bones.h"
#include "Bomb.h"
#include "utils.h"
using namespace utils;

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

	m_pText = new Texture("Score: " + std::to_string(m_Score), "DIN-Light.otf", 40.f, Color4f{ 1.f, 1.f, 1.f, 1.f });
	m_pGameOver = new Texture("GAME OVER", "DIN-Light.otf", 150.f, Color4f{ 0.f, 0.f, 0.f, 1.f });
	m_pGameOver2 = new Texture("Press R to restart", "DIN-Light.otf", 40.f, Color4f{ 0.f, 0.f, 0.f, 1.f });
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

	delete m_pText;
	m_pText = nullptr;
	delete m_pGameOver;
	m_pGameOver = nullptr;
	delete m_pGameOver2;
	m_pGameOver2 = nullptr;
}

void Game::Update( float elapsedSec )
{	
	if (!m_IsDead)
	{
		m_Timer += elapsedSec;

		if (m_Timer >= 2.f)
		{
			CreateObject();
			m_Timer = 0;
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
				}
				m_Bombs[idx]->Update(elapsedSec);
				if (m_Bombs[idx]->IsDestroyed())
				{
					delete m_Bombs[idx];
					m_Bombs[idx] = nullptr;
				}
			}
		}
	}	
	
}

void Game::Draw( ) const
{
	ClearBackground();	

	m_pText->Draw(Point2f{ GetViewPort().width / 2 - 55.f, GetViewPort().height - 45.f });

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
	
	if (!m_IsDead)
	{
		SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });

		//DrawRect(m_Position, 25.f, 50.f, 2.f);
		FillEllipse(Point2f{ m_Position.x + 12.5f, m_Position.y + 40.f }, 5.f, 5.f);
		utils::DrawLine(Point2f{ m_Position.x + 12.5f, m_Position.y + 40.f }, Point2f{ m_Position.x + 12.5f, m_Position.y + 20.f }, 2.f);
		utils::DrawLine(Point2f{ m_Position.x + 5.f, m_Position.y + 6.f }, Point2f{ m_Position.x + 12.5f, m_Position.y + 20.f }, 2.f);
		utils::DrawLine(Point2f{ m_Position.x + 20.f, m_Position.y + 6.f }, Point2f{ m_Position.x + 12.5f, m_Position.y + 20.f }, 2.f);
		utils::DrawLine(Point2f{ m_Position.x + 5.f, m_Position.y + 25.f }, Point2f{ m_Position.x + 12.5f, m_Position.y + 35.f }, 2.f);
		utils::DrawLine(Point2f{ m_Position.x + 20.f, m_Position.y + 25.f }, Point2f{ m_Position.x + 12.5f, m_Position.y + 35.f }, 2.f);
	}
	else
	{
		m_pGameOver->Draw(Point2f{ GetViewPort().width / 2 - 400.f, GetViewPort().height / 2 - 100.f });
		m_pGameOver2->Draw(Point2f{ GetViewPort().width / 2 - 150.f, GetViewPort().height / 2 - 150.f });
	}
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	if (!m_IsDead)
	{
		if (e.keysym.sym == SDLK_LEFT)
		{
			m_Position.x += -5.f;
		}
		else if (e.keysym.sym == SDLK_RIGHT)
		{
			m_Position.x += 5.f;
		}

		if (e.keysym.sym == SDLK_DOWN)
		{
			m_Position.y += -5.f;
		}
		else if (e.keysym.sym == SDLK_UP)
		{
			m_Position.y += 5.f;
		}

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
		std::cout << "New Bone created!" << std::endl;
	}
	else 
	{
		m_Bombs.push_back(new Bomb(GetViewPort().width, GetViewPort().height));
		std::cout << "New Bomb created!" << std::endl;
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

	m_Score = 0;
	m_Timer = 0;
	m_IsDead = false;

	delete m_pText;
	m_pText = nullptr;
	m_pText = new Texture("Score: " + std::to_string(m_Score), "DIN-Light.otf", 40.f, Color4f{ 1.f, 1.f, 1.f, 1.f });
}
