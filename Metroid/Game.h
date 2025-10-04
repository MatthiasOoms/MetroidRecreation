#pragma once
#include <vector>

class Level;
class Avatar;
class Enemy;
class PowerUp;
class PickUp;
class Tile;
class Door;
class Quicksand;
class Camera;
class HUD;
class Room;
class ZebSpawner;
class SoundStream;

class Game final
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	~Game();

	void Update( float elapsedSec );
	void Draw( ) const;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e );
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e );
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e );
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e );
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e );

private:
	// DATA MEMBERS
	const Window m_Window;

	const float m_Scale;

	SoundStream* m_pBackTrack;
	const Level* m_pLevel;
	Avatar* m_pAvatar;
	std::vector<Enemy*> m_pEnemies;
	std::vector<PowerUp*> m_pPowerUps;
	std::vector<PickUp*> m_pPickUps;
	std::vector<Tile*> m_pTiles;
	std::vector<Door*> m_pDoors;
	std::vector<Quicksand*> m_pQuicksand;
	std::vector<Room*> m_pRooms;
	std::vector<ZebSpawner*> m_pZebSpawners;
	Camera* m_pCamera;
	HUD* m_pHUD;

	// FUNCTIONS
	void Initialize( );
	void Cleanup( );
	void ClearBackground( ) const;
};