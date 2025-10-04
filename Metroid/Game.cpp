#pragma once
#include "pch.h"
#include "Game.h"

#include "Level.h"
#include "Avatar.h"
#include "PowerUp.h"
#include "Tile.h"
#include "Door.h"
#include "Quicksand.h"
#include "Room.h"

#include "Enemy.h"
#include "Zoomer.h"
#include "Skree.h"
#include "Reo.h"
#include "Ripper.h"
#include "Waver.h"
#include "Zeb.h"
#include "Mellow.h"

#include "ZebSpawner.h"
#include "SkreeBall.h"

#include "SoundStream.h"

#include "Camera.h"
#include "HUD.h"
#include "utils.h"
#include "SVGParser.h"

Game::Game( const Window& window ) 
	: m_Scale{ 3 }
	, m_Window{ window }
	, m_pLevel{ new Level() }
	, m_pAvatar{ new Avatar() }
	, m_pCamera{ new Camera(m_Window.width/m_Scale, m_Window.height/m_Scale) }
	, m_pHUD{ new HUD{} }
{
	Initialize( );
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	SDL_ShowCursor(SDL_DISABLE);

	// Why is this not working? File wrong?
	m_pBackTrack = new SoundStream{"Resources/Sound/BrinstarTheme.mp3"};

	m_pRooms.push_back(new Room(Point2f(64, 1265), 5, 1));	
	m_pRooms.push_back(new Room(Point2f(64 + 5 * 256, 1265 - 5 * 240), 1, 6));

	m_pEnemies.push_back(new Zoomer(Point2f(656, 1456), Zoomer::Type::orange, Zoomer::Direction::clockwise));
	m_pEnemies.push_back(new Zoomer(Point2f(756, 1456), Zoomer::Type::orange, Zoomer::Direction::clockwise));
	m_pEnemies.push_back(new Zoomer(Point2f(560, 1380), Zoomer::Type::orange, Zoomer::Direction::counterclockwise));

	m_pEnemies.push_back(new Ripper(Point2f(1000, 1330), Ripper::Type::orange));

	m_pEnemies.push_back(new Waver(Point2f(1000, 1300), Waver::Type::green));

	m_pEnemies.push_back(new Skree(Point2f(1000, 1450), Skree::Type::green));

	m_pEnemies.push_back(new Reo(Point2f(1050, 1450), Reo::Type::orange));

	m_pEnemies.push_back(new Mellow(Point2f(1100, 1450)));

	m_pZebSpawners.push_back(new ZebSpawner(Point2f(950, 1290)));

	m_pPowerUps.push_back(new PowerUp(PowerUp::Type::ball, Point2f(850, 1345)));
	m_pPowerUps.push_back(new PowerUp(PowerUp::Type::bombs, Point2f(882, 1345)));
	m_pPowerUps.push_back(new PowerUp(PowerUp::Type::energy, Point2f(914, 1345)));
	m_pPowerUps.push_back(new PowerUp(PowerUp::Type::missiles, Point2f(946, 1345)));
	m_pPowerUps.push_back(new PowerUp(PowerUp::Type::varia, Point2f(978, 1345)));
	m_pPowerUps.push_back(new PowerUp(PowerUp::Type::icebeam, Point2f(1010, 1345)));
	m_pPowerUps.push_back(new PowerUp(PowerUp::Type::longbeam, Point2f(1042, 1345)));

	m_pTiles.push_back(new Tile(Point2f(800, 1295), Tile::Type::blueRubble));
	m_pTiles.push_back(new Tile(Point2f(800, 1311), Tile::Type::greenRubble));

	m_pQuicksand.push_back(new Quicksand{ Point2f(816, 1295), 5 });

	m_pDoors.push_back(new Door(Point2f(1200, 1295), Door::Type::blue));
	m_pDoors.push_back(new Door(Point2f(1312, 1376), Door::Type::red));
}

void Game::Cleanup( )
{
	delete m_pLevel;
	m_pLevel = nullptr;

	delete m_pAvatar;
	m_pAvatar = nullptr;
	
	delete m_pCamera;
	m_pCamera = nullptr;

	delete m_pHUD;
	m_pHUD = nullptr;
	
	for (int i{}; i < m_pEnemies.size(); ++i)
	{
		delete m_pEnemies[i];
		m_pEnemies[i] = nullptr;
	}

	for (int i{}; i < m_pPowerUps.size(); ++i)
	{
		delete m_pPowerUps[i];
		m_pPowerUps[i] = nullptr;
	}

	for (int i{}; i < m_pTiles.size(); ++i)
	{
		delete m_pTiles[i];
		m_pTiles[i] = nullptr;
	}

	for (int i{}; i < m_pDoors.size(); ++i)
	{
		delete m_pDoors[i];
		m_pDoors[i] = nullptr;
	}

	for (int i{}; i < m_pQuicksand.size(); ++i)
	{
		delete m_pQuicksand[i];
		m_pQuicksand[i] = nullptr;
	}

	for (int i{}; i < m_pPickUps.size(); ++i)
	{
		delete m_pPickUps[i];
		m_pPickUps[i] = nullptr;
	}

	for (int i{}; i < m_pRooms.size(); ++i)
	{
		delete m_pRooms[i];
		m_pRooms[i] = nullptr;
	}

	for (int i{}; i < m_pZebSpawners.size(); ++i)
	{
		delete m_pZebSpawners[i];
		m_pZebSpawners[i] = nullptr;
	}

	delete m_pBackTrack;
	m_pBackTrack = nullptr;
}

void Game::Update( float elapsedSec )
{
	if (!m_pBackTrack->IsPlaying())
	{
		m_pBackTrack->Play(true);
	}

	for (size_t i{}; i < m_pRooms.size(); ++i)
	{
		if (m_pRooms[i])
		{
			m_pRooms[i]->Update(m_pAvatar);

			if (m_pRooms[i]->GetActive())
			{
				m_pCamera->SetLevelBoundaries(m_pRooms[i]->GetShape());
			}
		}
	}

	if (!m_pAvatar->GetTransforming())
	{
		for (int i{}; i < m_pZebSpawners.size(); ++i)
		{
			if (m_pZebSpawners[i])
			{
				m_pZebSpawners[i]->Update(elapsedSec, m_pAvatar, m_pLevel, m_pTiles);
			}
		}

		for (int i{}; i < m_pEnemies.size(); ++i)
		{
			if (m_pEnemies[i] != nullptr)
			{
				if (abs(m_pEnemies[i]->GetShape().left - m_pAvatar->GetShape().left) < 200 && abs(m_pEnemies[i]->GetShape().bottom - m_pAvatar->GetShape().bottom) < 200) // If enemy is in camera range (200px)
				{
					if (!m_pEnemies[i]->IsFrozen())
					{
						m_pEnemies[i]->Update(elapsedSec, m_pLevel, m_pTiles);

						Skree* pSkree{ dynamic_cast<Skree*>(m_pEnemies[i]) };
						if (pSkree)
						{
							pSkree->Attack(m_pAvatar->GetShape());

							for (size_t i{}; i < pSkree->GetSkreeBalls().size(); i++)
							{
								if (pSkree->GetSkreeBalls()[i])
								{
									if (utils::IsOverlapping(pSkree->GetSkreeBalls()[i]->GetShape(), m_pAvatar->GetShape()))
									{
										m_pAvatar->Damage(3);

										pSkree->GetSkreeBalls()[i]->Collision();
									}
								}
							}
						}

						Reo* pReo{ dynamic_cast<Reo*>(m_pEnemies[i]) };
						if (pReo)
						{
							pReo->Attack(elapsedSec, m_pAvatar->GetShape());
						}

						Mellow* pMellow{ dynamic_cast<Mellow*>(m_pEnemies[i]) };
						if (pMellow)
						{
							pMellow->Attack(elapsedSec, m_pAvatar->GetShape());
						}

						if (m_pEnemies[i]->GetHealth() <= 0)
						{
							// Enemy drops
							m_pPickUps.push_back(new PickUp{ Point2f(m_pEnemies[i]->GetShape().left, m_pEnemies[i]->GetShape().bottom), m_pAvatar->HasMissiles()});
							// Enemy death
							delete m_pEnemies[i];
							m_pEnemies[i] = nullptr;
						}
					}
				}
			}
		}
	}

	m_pAvatar->Update(elapsedSec, m_pLevel, m_pEnemies, m_pTiles, m_pDoors, m_pPickUps, m_pZebSpawners);
	
	m_pHUD->Update(m_pAvatar->GetEnergy(), m_pAvatar->GetEnergyPacks(), m_pAvatar->GetMissiles(), m_pAvatar->HasMissiles());

	for (size_t i{}; i < m_pPowerUps.size(); ++i)
	{
		if (m_pPowerUps[i] != nullptr)
		{
			m_pPowerUps[i]->Update(m_pAvatar);
		}
	}

	for (size_t i{}; i < m_pPickUps.size(); ++i)
	{
		if (m_pPickUps[i])
		{
			if (m_pPickUps[i]->GetType() != PickUp::Type::nothing)
			{
				m_pPickUps[i]->Update(m_pAvatar, elapsedSec);
			}
			else
			{
				delete m_pPickUps[i];
				m_pPickUps[i] = nullptr;
			}
		}
	}

	for (size_t i{}; i < m_pQuicksand.size(); ++i)
	{
		m_pQuicksand[i]->Update(m_pAvatar);
	}

	// Check keyboard state
	//const Uint8 *pStates = SDL_GetKeyboardState( nullptr );
	//if ( pStates[SDL_SCANCODE_RIGHT] )
	//{
	//	std::cout << "Right arrow key is down\n";
	//}
	//if ( pStates[SDL_SCANCODE_LEFT] && pStates[SDL_SCANCODE_UP])
	//{
	//	std::cout << "Left and up arrow keys are down\n";
	//}
}

void Game::Draw( ) const
{
	ClearBackground( );

	glPushMatrix();

		glScalef(m_Scale, m_Scale, 1);

		m_pCamera->Transform(m_pAvatar->GetCameraShape());

		m_pLevel->Draw();

		for (int i{}; i < m_pEnemies.size(); i++)
		{
			if (m_pEnemies[i] != nullptr)
			{
				if (abs(m_pEnemies[i]->GetShape().left - m_pAvatar->GetShape().left) < 200 && abs(m_pEnemies[i]->GetShape().bottom - m_pAvatar->GetShape().bottom) < 200) // If enemy is in camera range (200px)
				{
					m_pEnemies[i]->Draw();
				}
			}
		}

		for (int i{}; i < m_pZebSpawners.size(); ++i)
		{
			if (m_pZebSpawners[i])
			{
				m_pZebSpawners[i]->Draw();
			}
		}

		for (int i{}; i < m_pPowerUps.size(); i++)
		{
			if (m_pPowerUps[i] != nullptr)
			{
				m_pPowerUps[i]->Draw();
			}
		}

		for (int i{}; i < m_pTiles.size(); i++)
		{
			if (m_pTiles[i] != nullptr)
			{
				m_pTiles[i]->Draw();
			}
		}

		for (int i{}; i < m_pDoors.size(); i++)
		{
			if (m_pDoors[i] != nullptr)
			{
				m_pDoors[i]->Draw();
			}
		}

		for (size_t i{}; i < m_pPickUps.size(); ++i)
		{
			if (m_pPickUps[i])
			{
				m_pPickUps[i]->Draw();
			}
		}

		m_pAvatar->Draw();

		for (size_t i{}; i < m_pQuicksand.size(); ++i)
		{
			if (m_pQuicksand[i])
			{
				m_pQuicksand[i]->Draw();
			}
		}

	glPopMatrix();

	// HUD
	glPushMatrix();

		glScalef(m_Scale, m_Scale, 1);

		if (m_pHUD)
		{
			float offset{ 32 };
			Point2f location{ offset, m_Window.height / m_Scale - offset };
			m_pHUD->DrawLarge(location);

			m_pHUD->DrawSmall(location);

			m_pHUD->DrawAmounts(location);
		}

	glPopMatrix();	
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	//switch ( e.keysym.sym )
	//{
	//case SDLK_LEFT:
	//	//std::cout << "Left arrow key released\n";
	//	break;
	//case SDLK_RIGHT:
	//	//std::cout << "`Right arrow key released\n";
	//	break;
	//case SDLK_1:
	//case SDLK_KP_1:
	//	//std::cout << "Key 1 released\n";
	//	break;
	//}
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
	glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}
