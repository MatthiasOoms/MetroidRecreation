#pragma once
#include "Vector2f.h"

class Texture;

class Projectile
{
public:
	Projectile(Vector2f bulletVelocity, Rectf playerShape, float lifespan);
	Projectile(const Projectile& other) = delete;
	Projectile& operator=(const Projectile& other) = delete;
	Projectile(Projectile&& other) = delete;
	Projectile& operator=(Projectile&& other) = delete;
	virtual ~Projectile();

	void Update(float elapsedSec);
	virtual void Draw(Point2f pos) const;
	bool HasExpired() const;
	Rectf GetShape() const;
	Vector2f GetVelocity() const;
	int GetFrame() const;
	int GetFrameCount() const;
	void Collision();

protected:
	// Movement
	Vector2f m_Velocity;
	Rectf m_Shape;
	float m_Lifespan;
	float m_Life;

	// Animation
	Texture* m_pTexture;
	float m_Rows, m_Cols;
	int m_Frame, m_NrOfFrames;
};

