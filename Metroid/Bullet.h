#pragma once
#include "Vector2f.h"
#include "Projectile.h"

class Texture;

class Bullet final : public Projectile
{
public:
	enum class Ammo
	{
		normal,
		ice
	};

	Bullet(Vector2f bulletVelocity, Rectf playerShape, Ammo ammoType, float lifespan);
	Bullet(const Bullet& other) = delete;
	Bullet& operator=(const Bullet& other) = delete;
	Bullet(Bullet&& other) = delete;
	Bullet& operator=(Bullet&& other) = delete;
	virtual ~Bullet() final;

	virtual void Draw(Point2f pos) const override;
	Ammo GetType() const;

private:
	// Animation
	Ammo m_Ammo;
};

