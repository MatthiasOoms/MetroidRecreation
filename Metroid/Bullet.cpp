#pragma once
#include "pch.h"
#include "Bullet.h"
#include "Texture.h"

Bullet::Bullet(Vector2f bulletVelocity, Rectf playerShape, Ammo ammoType, float lifespan)
	: Projectile(bulletVelocity, playerShape, lifespan)
{
	m_pTexture = new Texture("Resources/Bullets.png");
	m_Rows = 3;
	m_Ammo = ammoType;
	m_NrOfFrames = 1;

	if (m_Velocity.x > 0)
	{
		m_Shape = Rectf(playerShape.left + playerShape.width, playerShape.bottom + playerShape.height / 1.7f, 8, 8);
	}
	else if (m_Velocity.x < 0)
	{
		m_Shape = Rectf(playerShape.left - m_pTexture->GetWidth(), playerShape.bottom + playerShape.height / 1.7f, 8, 8);
	}
	else
	{
		m_Shape = Rectf(playerShape.left + m_pTexture->GetWidth() / 2, playerShape.bottom + playerShape.height, 8, 8);
	}
}

Bullet::~Bullet()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void Bullet::Draw(Point2f pos) const
{
	if (!HasExpired())
	{
		Rectf srcRect{ 0, (m_pTexture->GetHeight() / m_Rows) + (int)m_Ammo * (m_pTexture->GetHeight() / m_Rows), m_pTexture->GetWidth(), m_pTexture->GetHeight() / m_Rows };

		m_pTexture->Draw(pos, srcRect);
	}
	else
	{
		if (m_Frame <= m_NrOfFrames)
		{
			Rectf srcRect{ 
				m_Frame * (m_pTexture->GetWidth() / m_Cols),
				m_Rows * m_pTexture->GetHeight() / m_Rows,
				m_pTexture->GetWidth() / m_Cols,
				m_pTexture->GetHeight() / m_Rows };

			m_pTexture->Draw(pos, srcRect);
		}
	}
}

Bullet::Ammo Bullet::GetType() const
{
	return m_Ammo;
}
