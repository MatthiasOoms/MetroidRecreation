#include "pch.h"
#include "Avatar.h"
#include "Texture.h"
#include "Level.h"
#include "Bullet.h"
#include "Missile.h"
#include "Bomb.h"
#include "Tile.h"
#include "Door.h"
#include "utils.h"
#include "ZebSpawner.h"
#include "Zeb.h"
#include "SoundEffect.h"
#include <iostream>

Avatar::Avatar()
	// General
	: m_Energy{ 30 }
	, m_MaxEnergy { 99 }
	, m_Shape{ 705, 1311, 14, 30 } // 705, 1311 start pos
	, m_CameraShape{ Rectf(m_Shape.left - m_Shape.width, m_Shape.bottom - m_Shape.height, m_Shape.width * 2, m_Shape.height * 2) }
	, m_DamageTimer{ 0 }
	, m_DamageCooldown{ 0.5f }

	// Movement
	, m_HorSpeed{ 150 }
	, m_JumpSpeed{ 430/1.5f }
	, m_Velocity{ 0.f, 0.f }
	, m_Acceleration{ 0.f, -981.f/2 }
	, m_ActionState{ ActionState::spawn }
	, m_LookingLeft{ false }

	// Transformation
	, m_IsPickingUp{ false }
	, m_AccuTransformSec{ 0 }
	, m_MaxTransformSec{ 1 }

	// Animation
	, m_pSpritesTexture{ new Texture{"Resources/Samus.png"} }
	, m_ClipHeight{ m_pSpritesTexture->GetHeight() / 8 } // 8 rows
	, m_ClipWidth{ m_pSpritesTexture->GetWidth() / 16 } // 16 cols
	, m_NrOfFrames{ 1 }
	, m_NrFramesPerSec{ 30 }
	, m_AnimTime{}
	, m_AnimFrame{}
	, m_Somersault{ false }

	// Shooting
	, m_IsAimingUp{ false }
	, m_IsShooting{ false }
	, m_ShootTimer{ 0 }
	, m_ShootCooldown{ 0.15f }
	, m_ProjectileDuration{0.3f }

	// PowerUps
	, m_HasBall{ false }
	, m_HasMissiles{ false }
	, m_MissileCount{ 0 }
	, m_HasBombs{ false }
	, m_EnergyPacks{ 0 }
	, m_MaxEnergyPacks{ 6 }
	, m_HasVaria{ false }
	, m_HasLong{ false }
	, m_HasIce{ false }

	// Helper
	, m_ControlsPrinted{ false }

	// Sound
	, m_pBullet{ new SoundEffect{"Resources/Sound/Bullet.wav"} }
	, m_pMissile{ new SoundEffect{"Resources/Sound/Missile.wav"} }
	, m_pBomb{ new SoundEffect{"Resources/Sound/Bomb.wav"} }
	, m_pDamage{ new SoundEffect{"Resources/Sound/Damage.wav"} }
	, m_pJump{ new SoundEffect{"Resources/Sound/Jump.wav"} }
{
}

Avatar::~Avatar()
{
	delete m_pSpritesTexture;
	m_pSpritesTexture = nullptr;
	
	for (size_t i{}; i < m_pProjectiles.size(); ++i)
	{
		if (m_pProjectiles[i] != nullptr)
		{
			delete m_pProjectiles[i];
			m_pProjectiles[i] = nullptr;
		}
	}

	for (size_t i{}; i < m_pBombs.size(); ++i)
	{
		if (m_pBombs[i] != nullptr)
		{
			delete m_pBombs[i];
			m_pBombs[i] = nullptr;
		}
	}

	delete m_pBullet;
	m_pBullet = nullptr;

	delete m_pMissile;
	m_pMissile = nullptr;

	delete m_pBomb;
	m_pBomb = nullptr;

	delete m_pJump;
	m_pJump = nullptr;

	delete m_pDamage;
	m_pDamage = nullptr;
}


// Updates
void Avatar::Update(float elapsedSec, const Level* level, std::vector<Enemy*>& enemies, std::vector<Tile*>& tiles, std::vector<Door*>& doors, std::vector<PickUp*>& pickUps, std::vector<ZebSpawner*>& zebSpawners)
{
	// Animation
	FrameHandling(elapsedSec);

	// Handle State changes
	// Read keyboard
	const Uint8* pStates = SDL_GetKeyboardState( nullptr );

	if (m_DamageTimer <= m_DamageCooldown)
	{
		m_DamageTimer += elapsedSec;
	}
	if (m_ShootTimer <= m_ShootCooldown * 2)
	{
		m_ShootTimer += elapsedSec;
	}
	Vector2f projectileVelocity{};
	float projectileSpeed{ 150 };

	if (pStates[SDL_SCANCODE_I] && !m_ControlsPrinted)
	{
		m_ControlsPrinted = true;
		std::cout << "ARROW RIGHT: Right Movement \nARROW LEFT: Left Movement \nARROW DOWN: Duck/Roll \nARROW UP: Aim Up \nSPACEBAR: Jump \nZ: Shoot Bullets \nX: Shoot Missile/Drop Bomb";
	}

	switch (m_ActionState)
	{
	case Avatar::ActionState::idle:
		// Animations
		m_NrOfFrames = 1;

		// Reset velocity
		m_Velocity.x = 0;
		if (level->IsOnGround(m_Shape, m_Velocity))
		{
			m_Velocity.y = 0;
		}
		
		ActionStateChangesIdle(level, tiles, pStates);

		if (m_ActionState == ActionState::idle)
		{
			ShootingAnimation(pStates);

			if (!ShootBullets(pStates, projectileVelocity, projectileSpeed))
			{
				ShootMissiles(pStates, projectileVelocity, projectileSpeed);
			}
		}
		break;

	case Avatar::ActionState::walk:
		// Animations
		m_NrOfFrames = 3;

		ActionStateChangesWalk(level, pStates);

		if (m_ActionState == ActionState::walk)
		{
			ShootingAnimation(pStates);

			if (!ShootBullets(pStates, projectileVelocity, projectileSpeed))
			{
				ShootMissiles(pStates, projectileVelocity, projectileSpeed);
			}
		}
		break;

	case Avatar::ActionState::jump:
		// Animations
		m_NrOfFrames = 1;
		
		// Are we on the ground
		if (level->IsOnGround(m_Shape, m_Velocity))
		{
			Somersault(pStates);
		}
		// Animations
		else if (m_Somersault && (pStates[SDL_SCANCODE_LEFT] || pStates[SDL_SCANCODE_RIGHT]))
		{
			m_NrOfFrames = 4;
		}
		else
		{
			m_NrOfFrames = 1;
			m_Somersault = false;
		}

		Movement(pStates);

		if (level->IsOnGround(m_Shape, m_Velocity))
		{
			m_ActionState = ActionState::walk;
		}

		Jump(projectileVelocity, projectileSpeed, pStates);

		break;

	case Avatar::ActionState::duck:
		
		// Animations
		m_NrOfFrames = 3;

		Duck(elapsedSec);

		break;
	case Avatar::ActionState::roll:

		// Animations
		m_NrOfFrames = 4;

		Movement(pStates);

		Stand(pStates, level);

		DropBomb(pStates);

		break;
	case Avatar::ActionState::death:

		// Animations
		m_NrOfFrames = 1;

		m_Velocity.x = 0;
		m_Velocity.y = 0;

		break;
	case Avatar::ActionState::spawn:

		// Animations
		m_NrOfFrames = 1;

		SpawnTimer(elapsedSec);

		break;
	default:
		break;
	}

	// Transform
	TransformationTimer(elapsedSec);

	if (!m_IsPickingUp)
	{
		// Update
		EnemyUpdate(enemies, pStates);

		for (size_t i{}; i < zebSpawners.size(); ++i)
		{
			if (zebSpawners[i])
			{
				if (zebSpawners[i]->GetZeb())
				{
					if (zebSpawners[i]->GetZeb()->IsFrozen())
					{
						if (utils::IsOverlapping(zebSpawners[i]->GetZeb()->GetShape(), m_Shape))
						{
							SolidObjectCollision(zebSpawners[i]->GetZeb()->GetShape(), pStates);
						}
					}
				}
			}
		}

		ProjectileUpdate(level, doors, enemies, tiles, pickUps, zebSpawners, elapsedSec);

		BombsUpdate(tiles, enemies, pickUps, elapsedSec);

		TilesUpdate(tiles, enemies, elapsedSec, pStates);
	
		DoorsUpdate(doors, elapsedSec);

		MovementApplication(level, tiles, elapsedSec);
	}

	// Camera tracking
	if (!utils::IsOverlapping(m_Shape, m_CameraShape))
	{
		if (m_Shape.left < m_CameraShape.left)
		{
			m_CameraShape.left -= 2;
		}
		if (m_Shape.left + m_Shape.width > m_CameraShape.left)
		{
			m_CameraShape.left += 2;
		}
		if (m_Shape.bottom < m_CameraShape.bottom)
		{
			m_CameraShape.bottom -= 2;
		}
		if (m_Shape.bottom + m_Shape.height > m_CameraShape.bottom)
		{
			m_CameraShape.bottom += 2;
		}
	}

	// Ground/Platform collision
	level->HandleCollision(m_Shape, m_Velocity);
}

void Avatar::PowerUpHit(PowerUp::Type type)
{
	// Increment + State change
	m_IsPickingUp = true;
	SetPowerup(type);
}

void Avatar::PickUpHit(PickUp::Type type)
{
	// Increment + State change
	switch (type)
	{
	case PickUp::Type::nothing:
		break;
	case PickUp::Type::energySmall:
		if (m_Energy + 5 < m_MaxEnergy)
		{
			m_Energy += 5;
		}
		else
		{
			m_Energy = m_MaxEnergy;
		}
		break;
	case PickUp::Type::energyLarge:
		if (m_Energy + 20 < m_MaxEnergy)
		{
			m_Energy += 20;
		}
		else
		{
			m_Energy = m_MaxEnergy;
		}
		break;
	case PickUp::Type::missile:
		++m_MissileCount;
		break;
	default:
		break;
	}
}

void Avatar::FrameHandling(float elapsedSec)
{
	m_AnimTime += m_NrFramesPerSec * elapsedSec;
	m_AnimFrame = (int)m_AnimTime % m_NrOfFrames;
}

void Avatar::Damage(int value)
{
	m_pDamage->Play(0);
	if (m_Energy > 0)
	{
		if (m_DamageTimer > m_DamageCooldown)
		{
			m_DamageTimer = 0;

			if (m_HasVaria)
			{
				m_Energy -= value;
			}
			else
			{
				m_Energy -= value / 2;
			}
		}
	}
	else
	{
		m_ActionState = ActionState::death;
	}

}

void Avatar::MovementApplication(const Level* level, const std::vector<Tile*> tiles, float elapsedSec)
{
	// Apply movement
	if (!m_IsPickingUp && m_ActionState != ActionState::spawn && m_ActionState != ActionState::death)
	{
		// Apply Gravity to Player
		if (!level->IsOnGround(m_Shape, m_Velocity))
		{
			m_Velocity.y += m_Acceleration.y * elapsedSec;
		}

		// Movement application
		m_Shape.bottom += m_Velocity.y * elapsedSec;
		m_Shape.left += m_Velocity.x * elapsedSec;
	}
}

void Avatar::SetPowerup(PowerUp::Type type)
{
	switch (type)
	{
	case PowerUp::Type::ball:
		m_HasBall = true;
		break;
	case PowerUp::Type::missiles:
		m_HasMissiles = true;
		m_MissileCount += 5;
		break;
	case PowerUp::Type::energy:
		if (m_EnergyPacks < m_MaxEnergyPacks)
		{
			++m_EnergyPacks;
			m_MaxEnergy += 100;
		}
		m_Energy = m_MaxEnergy;
		break;
	case PowerUp::Type::bombs:
		m_HasBombs = true;
		break;
	case PowerUp::Type::varia:
		m_HasVaria = true;
		break;
	case PowerUp::Type::longbeam:
		m_HasLong = true;
		break;
	case PowerUp::Type::icebeam:
		m_HasIce = true;
		break;
	default:
		break;
	}
} 

void Avatar::ActionStateChangesWalk(const Level* level, const Uint8* pStates)
{
	// Left or Right?
	if (pStates[SDL_SCANCODE_RIGHT])
	{
		m_Velocity.x = m_HorSpeed;
		m_LookingLeft = false;
	}
	else if (pStates[SDL_SCANCODE_LEFT])
	{
		m_Velocity.x = -m_HorSpeed;
		m_LookingLeft = true;
	}
	// Neither
	else
	{
		m_ActionState = ActionState::idle;
	}

	// Jump?
	if (pStates[SDL_SCANCODE_SPACE])
	{
		m_pJump->Play(0);
		m_ActionState = ActionState::jump;
	}
}

void Avatar::ActionStateChangesIdle(const Level* level, const std::vector<Tile*> tiles, const Uint8* pStates)
{
	// Are we moving horizontally
	if (pStates[SDL_SCANCODE_RIGHT] || pStates[SDL_SCANCODE_LEFT])
	{
		m_ActionState = ActionState::walk;
	}

	// Are we on the ground
	if (level->IsOnGround(m_Shape, m_Velocity))
	{
		// Are we jumping
		if (pStates[SDL_SCANCODE_SPACE])
		{
			m_pJump->Play(0);
			m_ActionState = ActionState::jump;
		}
		else if (pStates[SDL_SCANCODE_DOWN] && m_HasBall)
		{
			m_ActionState = ActionState::duck;
		}
	}
}

void Avatar::Movement(const Uint8* pStates)
{
	if (!m_IsPickingUp)
	{
		if (pStates[SDL_SCANCODE_LEFT])
		{
			m_LookingLeft = true;
			m_Velocity.x = -m_HorSpeed;
		}
		else if (pStates[SDL_SCANCODE_RIGHT])
		{
			m_LookingLeft = false;
			m_Velocity.x = m_HorSpeed;
		}
		else
		{
			m_Velocity.x = 0;
		}
	}
}

void Avatar::Jump(Vector2f projectileVelocity, const float projectileSpeed, const Uint8* pStates)
{
	if (m_ActionState == ActionState::jump && !m_Somersault && !m_IsPickingUp)
	{
		ShootingAnimation(pStates);

		if (!ShootBullets(pStates, projectileVelocity, projectileSpeed))
		{
			ShootMissiles(pStates, projectileVelocity, projectileSpeed);
		}
	}
}

void Avatar::SolidObjectCollision(Rectf objectShape, const Uint8* pStates)
{
	// Frozen enemy Player collision
	if (m_Shape.bottom > objectShape.bottom + objectShape.height / 2)
	{
		m_Shape.bottom = objectShape.bottom + objectShape.height;

		if (m_ActionState == ActionState::jump && m_Velocity.y < 0)
		{
			m_ActionState = ActionState::idle;
		}
		if (m_Velocity.y < 0)
		{
			m_Velocity.y = 0;
		}
		if (pStates[SDL_SCANCODE_SPACE] && m_Shape.left > objectShape.left - objectShape.width + 2 && m_Shape.left < objectShape.left + objectShape.width - 2)
		{
			Somersault(pStates);
		}
		if (pStates[SDL_SCANCODE_DOWN] && m_HasBall && m_Shape.left > objectShape.left - objectShape.width + 2 && m_Shape.left < objectShape.left + objectShape.width - 2)
		{
			m_ActionState = ActionState::duck;
		}
	}
	else if (m_Shape.bottom + m_Shape.height < objectShape.bottom + objectShape.height / 2)
	{
		m_Shape.bottom = objectShape.bottom - m_Shape.height;
		m_Velocity.y = 0;
	}
	else if (m_Velocity.x > 0 && m_Shape.left < objectShape.left)
	{
		m_Shape.left = objectShape.left - m_Shape.width;
	}
	else if (m_Velocity.x < 0 && m_Shape.left > objectShape.left)
	{
		m_Shape.left = objectShape.left + objectShape.width;
	}
}


// Draw
void Avatar::Draw() const
{
	DrawDirectionAvatar();
		
	DrawDirectionBullets();

	DrawDirectionMissiles();

	DrawBombs();
}


// Draw Avatar
void Avatar::DrawAvatar(Point2f pos) const
{	
	// Read keyboard
	if (m_DamageTimer > m_DamageCooldown / 2)
	{
		Rectf srcRect{ 0, 0, m_ClipWidth, m_ClipHeight };

		switch (m_ActionState)
		{
		case Avatar::ActionState::idle:

			PreJumpDraw(srcRect, pos);

			break;

		case Avatar::ActionState::walk:

			PreJumpDraw(srcRect, pos);

			break;

		case Avatar::ActionState::jump:

			JumpDraw(srcRect, pos);

			break;

		case Avatar::ActionState::duck:

			PostJumpDraw(srcRect, pos);

			break;

		case Avatar::ActionState::roll:

			PostJumpDraw(srcRect, pos);

			break;

		case Avatar::ActionState::death:

			PostJumpDraw(srcRect, pos);

			break;

		case Avatar::ActionState::spawn:

			PostJumpDraw(srcRect, pos);

			break;

		default:
			break;
		}
	}
}

void Avatar::PreJumpDraw(Rectf& srcRect, Point2f pos) const
{
	srcRect.bottom = ((int)m_ActionState + 1) * m_ClipHeight; // constant + 1
	srcRect.left = m_AnimFrame * m_ClipWidth;

	if (m_IsShooting)
	{
		srcRect.left += 128;
	}
	if (m_IsAimingUp)
	{
		srcRect.left += 256;
	}

	m_pSpritesTexture->Draw(pos, srcRect);
}

void Avatar::JumpDraw(Rectf& srcRect, Point2f pos) const
{
	srcRect.bottom = ((int)m_ActionState + 1) * m_ClipHeight; // constant + 1
	srcRect.left = m_AnimFrame * m_ClipWidth;

	if (m_IsShooting)
	{
		srcRect.left += 128;
	}
	if (m_IsAimingUp)
	{
		srcRect.left += 256;
	}

	if (m_Somersault)
	{
		srcRect.left = m_AnimFrame * m_ClipWidth;
		srcRect.bottom += m_ClipHeight;
	}
	else
	{
		srcRect.bottom = ((int)m_ActionState + 1) * m_ClipHeight; // constant + 1
	}

	m_pSpritesTexture->Draw(pos, srcRect);
}

void Avatar::PostJumpDraw(Rectf& srcRect, Point2f pos) const
{
	srcRect.bottom = ((int)m_ActionState + 2) * m_ClipHeight; // + 2 because constant +1 and past jump so +1
	srcRect.left = m_AnimFrame * m_ClipWidth;

	m_pSpritesTexture->Draw(pos, srcRect);
}


// Draw weapons
void Avatar::DrawMissiles(Point2f pos) const
{
	for (size_t i{}; i < m_pProjectiles.size(); ++i)
	{
		Missile* pMissile{ dynamic_cast<Missile*>(m_pProjectiles[i]) };
		if (pMissile != nullptr)
		{
			pMissile->Draw(pos);
		}
	}
}

void Avatar::DrawBombs() const
{
	for (size_t i{}; i < m_pBombs.size(); ++i)
	{
		if (m_pBombs[i] != nullptr)
		{
			m_pBombs[i]->Draw();
		}
	}
}

void Avatar::DrawDirectionAvatar() const
{
	// Check velocity direction
	if (!m_LookingLeft)
	{
		DrawAvatar(Point2f{ m_Shape.left, m_Shape.bottom });
	}
	else
	{
		glPushMatrix();

		glTranslatef(m_Shape.left, m_Shape.bottom, 0);
		glTranslatef(m_Shape.width, 0, 0);
		glScalef(-1, 1, 1);

		DrawAvatar(Point2f{ 0, 0 });

		glPopMatrix();
	}
}

void Avatar::DrawDirectionBullets() const
{
	for (size_t i{}; i < m_pProjectiles.size(); ++i)
	{
		Bullet* pBullet{ dynamic_cast<Bullet*>(m_pProjectiles[i]) };
		if (pBullet != nullptr)
		{
			Rectf bulletShape{ pBullet->GetShape() };
			Point2f pos{};

			if (pBullet->GetVelocity().x < 0)
			{
				glPushMatrix();

				glTranslatef(bulletShape.left, bulletShape.bottom, 0);
				glTranslatef(bulletShape.width, 0, 0);
				glScalef(-1, 1, 1);

				pBullet->Draw(Point2f(0,0));

				glPopMatrix();
			}
			else
			{
				pBullet->Draw(Point2f(bulletShape.left, bulletShape.bottom));
			}
			
		}
	}
}

void Avatar::DrawDirectionMissiles() const
{
	for (size_t i{}; i < m_pProjectiles.size(); ++i)
	{
		Missile* pMissile{ dynamic_cast<Missile*>(m_pProjectiles[i]) };
		if (pMissile != nullptr)
		{
			Rectf missileShape{ pMissile->GetShape() };

			if (pMissile->GetVelocity().x < 0)
			{
				glPushMatrix();

				glTranslatef(missileShape.left, missileShape.bottom, 0);
				glTranslatef(missileShape.width, 0, 0);
				glScalef(-1, 1, 1);

				DrawMissiles(Point2f{ 0, 0 });

				glPopMatrix();
			}
			else
			{
				DrawMissiles(Point2f{ missileShape.left, missileShape.bottom });
			}
		}
	}
}


// Animations
void Avatar::Somersault(const Uint8* pStates)
{
	// Somersault?
	if (pStates[SDL_SCANCODE_RIGHT])
	{
		// Animations
		m_NrOfFrames = 4;
		m_Somersault = true;

		m_LookingLeft = false;

		m_Velocity.x = m_HorSpeed;
	}
	else if (pStates[SDL_SCANCODE_LEFT])
	{
		// Animations
		m_NrOfFrames = 4;
		m_Somersault = true;

		m_LookingLeft = true;

		m_Velocity.x = -m_HorSpeed;
	}
	else
	{
		m_Velocity.x = 0;
		m_Somersault = false;
	}

	// Jump
	if (pStates[SDL_SCANCODE_SPACE])
	{
		// Jump
		m_Velocity.y = m_JumpSpeed;

		m_ActionState = ActionState::jump;
	}
}

void Avatar::ShootingAnimation(const Uint8* pStates)
{
	if (pStates[SDL_SCANCODE_UP])
	{
		m_IsAimingUp = true;
	}
	else
	{
		m_IsAimingUp = false;
	}

	// Shooting
	if (pStates[SDL_SCANCODE_Z] || pStates[SDL_SCANCODE_X])
	{
		m_IsShooting = true;
	}
	else
	{
		m_IsShooting = false;
	}
}

void Avatar::Duck(float elapsedSec)
{
	m_AccuTransformSec += elapsedSec;

	if (m_AccuTransformSec >= (float)m_NrOfFrames / (float)m_NrFramesPerSec)
	{
		m_Shape.height /= 2;
		
		m_AccuTransformSec = 0;
		m_ActionState = ActionState::roll;
	}
}

void Avatar::Stand(const Uint8* pStates, const Level* level)
{
	if (pStates[SDL_SCANCODE_UP])
	{
		if (level->CanStand(m_Shape))
		{
			m_Shape.height += m_Shape.height;
			m_ActionState = ActionState::idle;
		}
	}
}


// Timers
void Avatar::SpawnTimer(float elapsedSec)
{
	m_AccuTransformSec += elapsedSec;

	if (m_AccuTransformSec >= m_MaxTransformSec)
	{
		m_AccuTransformSec = 0;
		m_ActionState = ActionState::idle;
	}
}

void Avatar::TransformationTimer(float elapsedSec)
{
	if (m_IsPickingUp)
	{
		m_AccuTransformSec += elapsedSec;
		if (m_AccuTransformSec >= m_MaxTransformSec)
		{
			m_IsPickingUp = false;
			m_AccuTransformSec = 0;
		}
	}
}


// TODO: Enemy class is kind of enemyManager, rename?
// Enemies
void Avatar::EnemyUpdate(std::vector<Enemy*>& enemies, const Uint8* pStates)
{
	for (size_t i{}; i < enemies.size(); ++i)
	{
		if (enemies[i] != nullptr)
		{
			Rectf enemyShape{ enemies[i]->GetShape() };

			// Enemy Player collision
			if (utils::IsOverlapping(m_Shape, enemyShape))
			{
				if (enemies[i]->IsFrozen())
				{
					SolidObjectCollision(enemyShape, pStates);
				}
				else
				{
					// Player damage
					Damage(3);
				}
			}
		}
	}
}


// TODO: Implement projectileManager
// Projectiles
bool Avatar::ShootBullets(const Uint8* pStates, Vector2f projectileVelocity, const float projectileSpeed)
{
	if (pStates[SDL_SCANCODE_Z] && m_ShootTimer >= m_ShootCooldown)
	{
		m_pBullet->Play(0);
		m_ShootTimer = 0;

		if (!m_LookingLeft)
		{
			projectileVelocity.x = m_Velocity.x + projectileSpeed;
		}
		else
		{
			projectileVelocity.x = m_Velocity.x - projectileSpeed;
		}

		if (m_IsAimingUp)
		{
			projectileVelocity.x = 0;
			projectileVelocity.y = m_Velocity.y + projectileSpeed;
		}

		if (m_HasLong)
		{
			m_ProjectileDuration = 0.5f;
		}

		if (m_HasIce)
		{
			m_pProjectiles.push_back(new Bullet(projectileVelocity, m_Shape, Bullet::Ammo::ice, m_ProjectileDuration));
			return true;
		}
		else
		{
			m_pProjectiles.push_back(new Bullet(projectileVelocity, m_Shape, Bullet::Ammo::normal, m_ProjectileDuration));
			return true;
		}
	}
	return false;
}

void Avatar::ShootMissiles(const Uint8* pStates, Vector2f projectileVelocity, const float projectileSpeed)
{
	if (pStates[SDL_SCANCODE_X] && m_ShootTimer >= m_ShootCooldown * 2)
	{
		if (m_HasMissiles)
		{
			if (m_MissileCount > 0)
			{
				m_pMissile->Play(0);

				m_ProjectileDuration = 0.3f;
				m_ShootTimer = 0;

				if (!m_LookingLeft)
				{
					projectileVelocity.x = m_Velocity.x + projectileSpeed;
				}
				else
				{
					projectileVelocity.x = m_Velocity.x - projectileSpeed;
				}

				if (!m_IsAimingUp)
				{
					--m_MissileCount;
					m_pProjectiles.push_back(new Missile(projectileVelocity, m_Shape, m_ProjectileDuration));
				}
			}
		}
	}
}

void Avatar::ProjectileUpdate(const Level* level, std::vector<Door*>& doors, std::vector<Enemy*>& enemies, std::vector<Tile*>& tiles, std::vector<PickUp*>& pickUps, std::vector<ZebSpawner*>& zebSpawners, float elapsedSec)
{
	// Bullet update
	for (size_t i{}; i < m_pProjectiles.size(); ++i)
	{
		if (m_pProjectiles[i] != nullptr)
		{
			// Deletion
			if (m_pProjectiles[i]->HasExpired() && m_pProjectiles[i]->GetFrame() >= m_pProjectiles[i]->GetFrameCount())
			{
				delete m_pProjectiles[i];
				m_pProjectiles[i] = nullptr;
			}
			// Collisions
			else if (!m_pProjectiles[i]->HasExpired())
			{
				// Bullet wall collision
				level->HandleProjectileCollision(m_pProjectiles[i]);

				// Bullet enemy collision
				if (m_pProjectiles[i] != nullptr)
				{
					ProjectileEnemyCollision(enemies, pickUps, i);
				}
				if (m_pProjectiles[i] != nullptr)
				{
					ProjectileZebCollision(zebSpawners, pickUps, i);
				}
				// Bullet door collision
				if (m_pProjectiles[i] != nullptr)
				{
					ProjectileDoorCollision(doors, i);
				}
				// Bullet tile collision
				if (m_pProjectiles[i] != nullptr)
				{
					ProjectileTileCollision(tiles, i);
				}
			}
			if (m_pProjectiles[i] != nullptr)
			{
				// Bullet movement
				m_pProjectiles[i]->Update(elapsedSec);
			}
		}
	}
}

void Avatar::ProjectileEnemyCollision(std::vector<Enemy*>& enemies, std::vector<PickUp*>& pickUps, size_t i)
{
	for (size_t j{}; j < enemies.size(); ++j)
	{
		if (enemies[j] != nullptr)
		{
			if (m_pProjectiles[i] != nullptr)
			{
				if (utils::IsOverlapping(m_pProjectiles[i]->GetShape(), enemies[j]->GetShape()))
				{
					// Enemy projectile collision
					Bullet* pBullet = dynamic_cast<Bullet*>(m_pProjectiles[i]);
					if (pBullet)
					{
						// if ice bullet and unfrozen enemy, freeze
						if (pBullet->GetType() == Bullet::Ammo::ice && !enemies[j]->IsFrozen())
						{
							enemies[j]->Freeze();
						}
						// if enemy already frozen or ammo is not ice, damage
						else if(enemies[j]->GetHealth() > 0)
						{
							enemies[j]->Damage();
						}
						// else kill
						else
						{
							// Enemy drops
							pickUps.push_back(new PickUp{ Point2f(enemies[j]->GetShape().left, enemies[j]->GetShape().bottom), m_HasMissiles });
							// Enemy death
							delete enemies[j];
							enemies[j] = nullptr;
						}
					}
					// if not bullet, then missile, kill
					else
					{
						// Enemy drops
						pickUps.push_back(new PickUp{ Point2f(enemies[j]->GetShape().left, enemies[j]->GetShape().bottom), m_HasMissiles });
						// Enemy death
						delete enemies[j];
						enemies[j] = nullptr;
					}

					// Collsion
					m_pProjectiles[i]->Collision();
				}
			}
		}
	}
}

void Avatar::ProjectileZebCollision(std::vector<ZebSpawner*>& zebSpawners, std::vector<PickUp*>& pickUps, size_t i)
{
	for (size_t j{}; j < zebSpawners.size(); ++j)
	{
		if (zebSpawners[j] != nullptr)
		{
			if (zebSpawners[j]->GetZeb())
			{
				if (utils::IsOverlapping(m_pProjectiles[i]->GetShape(), zebSpawners[j]->GetZeb()->GetShape()))
				{
					// Enemy projectile collision
					Bullet* pBullet = dynamic_cast<Bullet*>(m_pProjectiles[i]);
					if (pBullet)
					{
						// if ice bullet and unfrozen enemy, freeze
						if (pBullet->GetType() == Bullet::Ammo::ice && !zebSpawners[j]->GetZeb()->IsFrozen())
						{
							zebSpawners[j]->GetZeb()->Freeze();
						}
						// if enemy already frozen or ammo is not ice, damage
						else if (zebSpawners[j]->GetZeb()->GetHealth() > 0)
						{
							zebSpawners[j]->GetZeb()->Damage();
							if (zebSpawners[j]->GetZeb()->GetHealth() == 0)
							{
								// Enemy drops
								pickUps.push_back(new PickUp{ Point2f(zebSpawners[j]->GetZeb()->GetShape().left, zebSpawners[j]->GetZeb()->GetShape().bottom), m_HasMissiles });
								// Enemy death
								delete zebSpawners[j]->GetZeb();
								zebSpawners[j]->GetZeb() = nullptr;
							}
						}
						// else kill
						else
						{
							// Enemy drops
							pickUps.push_back(new PickUp{ Point2f(zebSpawners[j]->GetZeb()->GetShape().left, zebSpawners[j]->GetZeb()->GetShape().bottom), m_HasMissiles });
							// Enemy death
							delete zebSpawners[j]->GetZeb();
							zebSpawners[j]->GetZeb() = nullptr;
						}
					}
					// if not bullet, then missile, kill
					else
					{
						// Enemy drops
						pickUps.push_back(new PickUp{ Point2f(zebSpawners[j]->GetZeb()->GetShape().left, zebSpawners[j]->GetZeb()->GetShape().bottom), m_HasMissiles });
						// Enemy death
						delete zebSpawners[j]->GetZeb();
						zebSpawners[j]->GetZeb() = nullptr;
					}

					// Collsion
					m_pProjectiles[i]->Collision();
				}
			}
		}
	}
}

void Avatar::ProjectileDoorCollision(std::vector<Door*>& doors, size_t i)
{
	for (size_t j{}; j < doors.size(); ++j)
	{
		if (doors[j] != nullptr)
		{
			Missile* pMissile{ dynamic_cast<Missile*>(m_pProjectiles[i]) };
			if (utils::IsOverlapping(m_pProjectiles[i]->GetShape(), doors[j]->GetShape()) && !doors[j]->IsOpen())
			{
				if (pMissile)
				{
					doors[j]->Open();
				}
				else if (doors[j]->GetType() == Door::Type::blue)
				{
					doors[j]->Open();
				}

				m_pProjectiles[i]->Collision();
			}
		}
	}
}

void Avatar::ProjectileTileCollision(std::vector<Tile*>& tiles, size_t i)
{
	for (size_t j{}; j < tiles.size(); ++j)
	{
		if (tiles[j] != nullptr)
		{
			if (utils::IsOverlapping(m_pProjectiles[i]->GetShape(), tiles[j]->GetShape()) && !tiles[j]->IsExploded())
			{
				tiles[j]->Explode();

				m_pProjectiles[i]->Collision();
			}
		}
	}
}


// TODO: Implement bombManager
// Bombs
void Avatar::DropBomb(const Uint8* pStates)
{
	if (pStates[SDL_SCANCODE_X] && m_ShootTimer >= m_ShootCooldown && m_HasBombs)
	{
		m_ShootTimer = 0;
		m_pBombs.push_back(new Bomb(m_Shape));
	}
}

void Avatar::BombsUpdate(std::vector<Tile*>& tiles, std::vector<Enemy*>& enemies, std::vector<PickUp*>& pickUps, float elapsedSec)
{
	// Bomb update
	for (size_t i{}; i < m_pBombs.size(); ++i)
	{
		if (m_pBombs[i] != nullptr)
		{
			// Bomb timer
			m_pBombs[i]->Update(elapsedSec);

			// Bomb detonation
			if (m_pBombs[i]->IsDetonated() && m_pBombs[i]->GetFrame() >= m_pBombs[i]->GetFrameCount())
			{
				Circlef explosion{ m_pBombs[i]->GetCircle() };

				// Explosion effects
				ExplosionPlayerOverlap(explosion);

				ExplosionTilesOverlap(tiles, explosion);

				ExplosionEnemiesOverlap(enemies, pickUps, explosion);

				m_pBomb->Play(0);

				// Deletion
				delete m_pBombs[i];
				m_pBombs[i] = nullptr;
			}
		}
	}
}

void Avatar::ExplosionPlayerOverlap(Circlef& explosion)
{
	// Player hitreg
	if (utils::IsOverlapping(m_Shape, explosion))
	{
		m_Velocity.y += 200;
	}
}

void Avatar::ExplosionTilesOverlap(std::vector<Tile*>& tiles, Circlef& explosion)
{
	// Tile hitreg
	for (size_t i{}; i < tiles.size(); ++i)
	{
		if (tiles[i] != nullptr)
		{
			// Tile destruction
			if (utils::IsOverlapping(tiles[i]->GetShape(), explosion))
			{
				tiles[i]->Explode();
			}
		}
	}
}

void Avatar::ExplosionEnemiesOverlap(std::vector<Enemy*>& enemies, std::vector<PickUp*>& pickUps, Circlef& explosion)
{
	// Tile hitreg
	for (size_t i{}; i < enemies.size(); ++i)
	{
		if (enemies[i] != nullptr)
		{
			// Tile destruction
			if (utils::IsOverlapping(enemies[i]->GetShape(), explosion))
			{
				// Enemy drops
				pickUps.push_back(new PickUp{ Point2f(enemies[i]->GetShape().left, enemies[i]->GetShape().bottom), m_HasMissiles });
				// Enemy death
				delete enemies[i];
				enemies[i] = nullptr;
			}
		}
	}
}


// TODO: Implement tileManager
// Tiles
void Avatar::TilesUpdate(std::vector<Tile*>& tiles, std::vector<Enemy*>& enemies, float elapsedSec, const Uint8* pStates)
{
	for (size_t i{}; i < tiles.size(); ++i)
	{
		if (tiles[i] != nullptr)
		{
			// Tile revive
			tiles[i]->Update(elapsedSec);

			// Tile Player collision
			if (!tiles[i]->IsExploded())
			{
				// Tile collision
				if (tiles[i]->FloorCollision(m_Shape, m_Velocity))
				{
					if (pStates[SDL_SCANCODE_LEFT] || pStates[SDL_SCANCODE_RIGHT])
					{
						m_ActionState = ActionState::walk;
					}
					else
					{
						m_ActionState = ActionState::idle;
					}
					if (pStates[SDL_SCANCODE_SPACE])
					{
						m_ActionState = ActionState::jump;
						m_Velocity.y = m_JumpSpeed;
					}
					else
					{
						m_Velocity.y = 0;
					}
				}
			}
			// TODO: Implement tile enemy collision

		}
	}
}


// TODO: Implement doorManager?
// Doors
void Avatar::DoorsUpdate(std::vector<Door*>& doors, float elapsedSec)
{
	for (size_t i{}; i < doors.size(); ++i)
	{
		if (doors[i] != nullptr)
		{
			// Door closing
			doors[i]->Update(elapsedSec);

			// Door collision
			Rectf doorShape{ doors[i]->GetShape() };

			if (utils::IsOverlapping(doorShape, m_Shape) && !doors[i]->IsOpen())
			{
				DoorCollision(doorShape);
			}
		}
	}
}

void Avatar::DoorCollision(Rectf& doorShape)
{
	if (m_Velocity.x > 0 && m_Shape.left < doorShape.left)
	{
		m_Shape.left = doorShape.left - m_Shape.width;
	}
	else if (m_Velocity.x < 0 && m_Shape.left > doorShape.left)
	{
		m_Shape.left = doorShape.left + doorShape.width;
	}
}


// Helper
int Avatar::GetEnergy() const
{
	return m_Energy;
}

int Avatar::GetEnergyPacks() const
{
	return m_EnergyPacks;
}

int Avatar::GetMissiles() const
{
	return m_MissileCount;
}

bool Avatar::HasMissiles() const
{
	return m_HasMissiles;
}

Rectf Avatar::GetCameraShape() const
{
	return m_CameraShape;
}

bool Avatar::GetTransforming() const
{
	return m_IsPickingUp;
}

Rectf Avatar::GetShape() const
{
	return m_Shape;
}
