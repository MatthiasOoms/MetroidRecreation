#include "pch.h"
#include "Projectile.h"
#include "Texture.h"

Projectile::Projectile(Vector2f bulletVelocity, Rectf playerShape, float lifespan)
	: m_Velocity{ bulletVelocity }
	, m_pTexture{}
	, m_Lifespan{ lifespan }
	, m_Life{ 0 }
	, m_Rows{ 1 }
	, m_Cols{ 1 }
	, m_Frame{ 0 }
	, m_NrOfFrames{ 1 }
{
}

Projectile::~Projectile()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void Projectile::Update(float elapsedSec)
{
	m_Life += elapsedSec;
	m_Shape.left += m_Velocity.x * elapsedSec;
	m_Shape.bottom += m_Velocity.y * elapsedSec;

	if (HasExpired())
	{
		if (m_Frame < m_NrOfFrames)
		{
			++m_Frame;
		}
	}
}

void Projectile::Draw(Point2f pos) const
{
	if (!HasExpired())
	{
		Rectf srcRect{ 0, m_pTexture->GetHeight() / m_Rows, m_pTexture->GetWidth(), m_pTexture->GetHeight() / m_Rows };

		m_pTexture->Draw(pos, srcRect);
	}
	else
	{
		if (m_Frame < m_NrOfFrames)
		{
			Rectf srcRect{ m_Frame * (m_pTexture->GetWidth() / m_Cols),
				m_Rows * m_pTexture->GetHeight() / m_Rows,
				m_pTexture->GetWidth() / m_Cols,
				m_pTexture->GetHeight() / m_Rows };

			m_pTexture->Draw(pos, srcRect);
		}
	}
}

bool Projectile::HasExpired() const
{
	if (m_Life >= m_Lifespan)
	{
		return true;
	}
	return false;
}

Rectf Projectile::GetShape() const
{
	return m_Shape;
}

Vector2f Projectile::GetVelocity() const
{
	return m_Velocity;
}

int Projectile::GetFrame() const
{
	return m_Frame;
}

int Projectile::GetFrameCount() const
{
	return m_NrOfFrames;
}

void Projectile::Collision()
{
	m_Life = m_Lifespan;
}
