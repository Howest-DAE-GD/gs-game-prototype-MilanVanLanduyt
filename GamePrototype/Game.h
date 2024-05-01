#pragma once
#include "BaseGame.h"
#include "Vector2f.h"
#include "vector"
#include "Texture.h"

class Bones;
class Bomb;
class Game : public BaseGame
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game();

	void Update( float elapsedSec ) override;
	void Draw( ) const override;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e ) override;
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e ) override;
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e ) override;

private:
	Vector2f m_Velocity;
	Point2f m_Position;
	Rectf m_Hitbox;
	bool m_IsDead;
	float m_Timer;
	int m_Score;

	Texture* m_pText;
	Texture* m_pGameOver;
	Texture* m_pGameOver2;

	std::vector<Bones*> m_Bones;
	std::vector<Bomb*> m_Bombs;

	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;
	void CreateObject();
	void ResetGame();
};