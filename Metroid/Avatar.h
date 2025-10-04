#pragma once
#include "Vector2f.h"
#include "PowerUp.h"
#include "PickUp.h"
#include <vector>

class Level;
class Texture;
class Projectile;
class Bomb;
class Enemy;
class Tile;
class Door;
class ZebSpawner;
class SoundEffect;

class Avatar
{
public:
	Avatar();
	Avatar(const Avatar& other) = delete;
	Avatar& operator=(const Avatar& other) = delete;
	Avatar(Avatar&& other) = delete;
	Avatar& operator=(Avatar&& other) = delete;
	~Avatar();

	void Update(float elapsedSec, const Level* level, std::vector<Enemy*>& enemies, std::vector<Tile*>& tiles, std::vector<Door*>& doors, std::vector<PickUp*>& pickUps, std::vector<ZebSpawner*>& zebSpawners);
	void Draw() const;
	void PowerUpHit(PowerUp::Type type);
	void PickUpHit(PickUp::Type type);
	void Damage(int value);
	Rectf GetShape() const;
	int GetEnergy() const;
	int GetEnergyPacks() const;
	int GetMissiles() const;
	bool HasMissiles() const;
	Rectf GetCameraShape() const;
	bool GetTransforming() const;

private:
	enum class ActionState 
	{
		idle,
		walk,
		jump,
		duck,
		roll,
		death,
		spawn
	};

	// Player
	int m_Energy;
	int m_MaxEnergy;
	Rectf m_Shape;
	Rectf m_CameraShape;
	float m_DamageTimer;
	float m_DamageCooldown;

	// Movement
	float m_HorSpeed;
	float m_JumpSpeed;
	Vector2f m_Velocity;
	const Vector2f m_Acceleration;
	ActionState m_ActionState;

	// Transformations
	bool m_IsPickingUp;
	float m_AccuTransformSec;
	const float m_MaxTransformSec;

	// Animations
	Texture* m_pSpritesTexture;
	float m_ClipHeight;
	float m_ClipWidth;
	int m_NrOfFrames;
	int m_NrFramesPerSec;
	float m_AnimTime;
	int m_AnimFrame;

	// Jump
	bool m_Somersault;

	// Shooting
	bool m_IsAimingUp;
	bool m_IsShooting;
	float m_ShootTimer;
	float m_ShootCooldown;
	float m_ProjectileDuration;
	std::vector<Projectile*> m_pProjectiles;
	std::vector<Bomb*> m_pBombs;
	bool m_LookingLeft;

	// PowerUps
	bool m_HasBall;
	bool m_HasMissiles;
	int m_MissileCount;
	bool m_HasBombs;
	int m_EnergyPacks;
	int m_MaxEnergyPacks;
	bool m_HasVaria;
	bool m_HasLong;
	bool m_HasIce;

	bool m_ControlsPrinted;

	// Sound
	SoundEffect* m_pDamage;
	SoundEffect* m_pMissile;
	SoundEffect* m_pBullet;
	SoundEffect* m_pBomb;
	SoundEffect* m_pJump;

	// Draw helper functions
	void DrawAvatar(Point2f pos) const;
	void DrawDirectionAvatar() const;
	void DrawDirectionBullets() const;
	void DrawMissiles(Point2f pos) const;
	void DrawDirectionMissiles() const;
	void DrawBombs() const;
	void PreJumpDraw(Rectf& srcRect, Point2f pos) const;
	void JumpDraw(Rectf& srcRect, Point2f pos) const;
	void PostJumpDraw(Rectf& srcRect, Point2f pos) const;

	// Update helper functions
	void FrameHandling(float elapsedSec);

	// ActionState switch
	void ActionStateChangesIdle(const Level* level, const std::vector<Tile*> tiles, const Uint8* pStates);
	void ActionStateChangesWalk(const Level* level, const Uint8* pStates);
	void ShootingAnimation(const Uint8* pStates);
	bool ShootBullets(const Uint8* pStates, Vector2f projectileVelocity, const float projectileSpeed);
	void ShootMissiles(const Uint8* pStates, Vector2f projectileVelocity, const float projectileSpeed);
	void Somersault(const Uint8* pStates);
	void Movement(const Uint8* pStates);
	void Jump(Vector2f projectileVelocity, const float projectileSpeed, const Uint8* pStates);
	void Duck(float elapsedSec);
	void Stand(const Uint8* pStates, const Level* level);
	void DropBomb(const Uint8* pStates);
	void SpawnTimer(float elapsedSec);
	void TransformationTimer(float elapsedSec);

	// General Update
	void EnemyUpdate(std::vector<Enemy*>& enemies, const Uint8* pStates);
	void SolidObjectCollision(Rectf objectShape, const Uint8* pStates);
	void ProjectileUpdate(const Level* level, std::vector<Door*>& doors, std::vector<Enemy*>& enemies, std::vector<Tile*>& tiles, std::vector<PickUp*>& pickUps, std::vector<ZebSpawner*>& zebSpawners, float elapsedSec);
	void ProjectileEnemyCollision(std::vector<Enemy*>& enemies, std::vector<PickUp*>& pickUps, size_t i);
	void ProjectileZebCollision(std::vector<ZebSpawner*>& zebSpawners, std::vector<PickUp*>& pickUps, size_t i);
	void ProjectileDoorCollision(std::vector<Door*>& doors, size_t i);
	void ProjectileTileCollision(std::vector<Tile*>& tiles, size_t i);
	void BombsUpdate(std::vector<Tile*>& tiles, std::vector<Enemy*>& enemies, std::vector<PickUp*>& pickUps, float elapsedSec);
	void ExplosionPlayerOverlap(Circlef& explosion);
	void ExplosionTilesOverlap(std::vector<Tile*>& tiles, Circlef& explosion);
	void ExplosionEnemiesOverlap(std::vector<Enemy*>& enemies, std::vector<PickUp*>& pickUps, Circlef& explosion);
	void TilesUpdate(std::vector<Tile*>& tiles, std::vector<Enemy*>& enemies, float elapsedSec, const Uint8* pStates);
	void DoorsUpdate(std::vector<Door*>& doors, float elapsedSec);
	void DoorCollision(Rectf& doorShape);
	void MovementApplication(const Level* level, const std::vector<Tile*> tiles, float elapsedSec);

	void SetPowerup(PowerUp::Type);

};

