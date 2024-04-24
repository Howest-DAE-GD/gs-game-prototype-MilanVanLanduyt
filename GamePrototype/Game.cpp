#include "pch.h"
#include "Game.h"
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
	m_Position = Point2f{ GetViewPort().width / 2 - 12.5f, GetViewPort().height / 2 - 12.5f };
	m_Bool = false;
}

void Game::Cleanup( )
{
}

void Game::Update( float elapsedSec )
{
	m_Position.x += m_Velocity.x * elapsedSec;
	m_Position.y += m_Velocity.y * elapsedSec;
}

void Game::Draw( ) const
{
	ClearBackground();
	if (m_Bool)
	{
		SetColor(Color4f{ 0.f, 0.f, 0.f, 1.f });
	}
	else SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });

	const float width{ 25.f };

	FillEllipse(m_Position, width, width);
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	if (e.keysym.sym == SDLK_UP)
	{
		m_Velocity.y = 60.f;
	}
	else if (e.keysym.sym == SDLK_DOWN)
	{
		m_Velocity.y = -60.f;
	}
	else m_Velocity.y = 0;

	if (e.keysym.sym == SDLK_RIGHT)
	{
		m_Velocity.x = 60.f;
	}
	else if (e.keysym.sym == SDLK_LEFT)
	{
		m_Velocity.x = -60.f;
	}
	else m_Velocity.x = 0;
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{

	if (e.keysym.sym == SDLK_SPACE)
	{
		m_Bool = !m_Bool;
	}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONDOWN event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
	
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ClearBackground( ) const
{
	if (m_Bool)
	{
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear( GL_COLOR_BUFFER_BIT );
}
