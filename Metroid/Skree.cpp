// TODO: Make Skree enemy
// Hangs on ceiling, if player close, go down (almost straight), after time explode in 4 directions (projectiles)
#include "pch.h"
#include "Skree.h"
#include "Level.h"
#include "Texture.h"
#include "Tile.h"
#include "utils.h"
#include "SkreeBall.h"

Skree::Skree(Point2f botLeft, Type type)
	: Enemy()
	, m_Type{ type }
	, m_IsDead{ false }
	, m_Life{ 0 }
	, m_LifeSpan{ 1 }
	, m_HasShot{ false }
	, m_IsAttacking{ false }
{
	m_Shape = Rectf(botLeft.x, botLeft.y, 32, 24);
	m_pTexture = new Texture("Resources/Skree.png");
	m_TextRows = 4;
	m_TextCols = 4;
	m_ClipHeight = m_pTexture->GetHeight() / m_TextRows;
	m_ClipWidth = m_pTexture->GetWidth() / m_TextCols;
	m_NrOfFrames = 3;
	m_NrFramesPerSec = 8;
	m_AnimTime = 0;
	m_AnimFrame = 0;
	m_Velocity.y = 5;
}

Skree::~Skree()
{
	delete m_pTexture;
	m_pTexture = nullptr;

	for (size_t i{}; i < m_pProjectiles.size(); ++i)
	{
		if (m_pProjectiles[i])
		{
			delete m_pProjectiles[i];
			m_pProjectiles[i] = nullptr;
		}
	}
}

void Skree::Update(float elapsedSec, const Level* level, const std::vector<Tile*>& tiles)
{
	if (m_IsDead)
	{
		if (!m_IsFrozen)
		{
			FrameHandling(elapsedSec);
			Explode(elapsedSec);
		}
	}
	else
	{
		if (!m_IsFrozen)
		{
			FrameHandling(elapsedSec);
		}

		if (level->HandleSwoopersCollision(m_Shape, m_Velocity))
		{
			m_Velocity.y = 0;
			Death();
		}

		m_Shape.left += m_Velocity.x * elapsedSec;
		m_Shape.bottom += m_Velocity.y * elapsedSec;

		for (size_t i{}; i < tiles.size(); ++i)
		{
			if (tiles[i])
			{
				if (!tiles[i]->IsExploded())
				{
					if (tiles[i]->FloorCollision(m_Shape, m_Velocity))
					{
						m_Velocity.y = 0;
						Death();
					}
				}
			}
		}
	}

	if (m_pProjectiles.size() > 0)
	{
		for (size_t i{}; i < m_pProjectiles.size(); ++i)
		{
			if (m_pProjectiles[i])
			{
				m_pProjectiles[i]->Update(elapsedSec);

				if (m_pProjectiles[i])
				{
					if (m_pProjectiles[i]->HasExpired())
					{
						delete m_pProjectiles[i];
						m_pProjectiles[i] = nullptr;
					}
				}
			}
		}
	}
}

void Skree::DrawSkree(Point2f pos) const
{
	Rectf srcRect{ 0, m_ClipHeight, m_ClipWidth, m_ClipHeight };

	srcRect.bottom = ((int)m_Type + 1) * m_ClipHeight;
	if (IsFrozen())
	{
		srcRect.bottom = 0;
	}

	srcRect.width *= 2;
	srcRect.left = 0;

	m_pTexture->Draw(pos, srcRect);
}

void Skree::DrawSkreeHalf(Point2f pos) const
{
	Rectf srcRect{ 0, m_ClipHeight, m_ClipWidth, m_ClipHeight };
	pos.x += srcRect.width / 2;

	srcRect.bottom = ((int)m_Type + 1) * m_ClipHeight;
	if (IsFrozen())
	{
		srcRect.bottom = 0;
	}
	
	srcRect.left = ((m_AnimFrame % 2) + 2) * m_ClipWidth;

	m_pTexture->Draw(pos, srcRect);
}

void Skree::Death()
{
	m_IsDead = true;
}

void Skree::Explode(float elapsedSec)
{
	m_Life += elapsedSec;
	if (m_Life > m_LifeSpan && m_pProjectiles.size() < 1 && !m_HasShot)
	{
		m_pProjectiles.push_back(new SkreeBall{ Vector2f(100,0), m_Shape, 1 });
		m_pProjectiles.push_back(new SkreeBall{ Vector2f(50,50), m_Shape, 1 });
		m_pProjectiles.push_back(new SkreeBall{ Vector2f(-100,0), m_Shape, 1 });
		m_pProjectiles.push_back(new SkreeBall{ Vector2f(-50,50), m_Shape, 1 });
		m_HasShot = true;
	}
}

bool Skree::GetDeath() const
{
	return m_IsDead;
}

void Skree::Attack(Rectf actorShape)
{
	if (abs(m_Shape.left - actorShape.left) < 20)
	{
		m_IsAttacking = true;

		m_Velocity.y = -200;
		if (actorShape.left < m_Shape.left)
		{
			m_Velocity.x = -10;
		}
		else
		{
			m_Velocity.x = 10;
		}
	}
	if (m_Life > m_LifeSpan && m_HasShot && !m_pProjectiles[0])
	{
		m_Health = 0;
	}
}

std::vector<SkreeBall*>& Skree::GetSkreeBalls()
{
	return m_pProjectiles;
}

void Skree::Draw() const
{
	for (size_t i{}; i < m_pProjectiles.size(); ++i)
	{
		if (m_pProjectiles[i])
		{
			m_pProjectiles[i]->Draw(Point2f(m_pProjectiles[i]->GetShape().left, m_pProjectiles[i]->GetShape().bottom));
		}
	}

	if (m_IsAttacking || m_IsDead)
	{
		DrawSkreeHalf(Point2f{ m_Shape.left, m_Shape.bottom });
	}
	else
	{
		DrawSkree(Point2f{ m_Shape.left, m_Shape.bottom });
	}
}
