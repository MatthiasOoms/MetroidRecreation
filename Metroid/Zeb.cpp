#include "pch.h"
#include "Zeb.h"
#include "Level.h"
#include "Texture.h"
#include "Tile.h"
#include "utils.h"
// TODO: Make Zeb enemy

Zeb::Zeb(Point2f botLeft, Rectf playerShape, Type type)
	: Enemy()
	, m_Type{ type }
	, m_LifeSpan{ 3 }
	, m_Life{ 0 }
{
	m_Shape = Rectf(botLeft.x, botLeft.y, 16, 16);
	m_pTexture = new Texture("Resources/Zeb.png");
	m_TextRows = 4;
	m_TextCols = 2;
	m_ClipHeight = m_pTexture->GetHeight() / m_TextRows;
	m_ClipWidth = m_pTexture->GetWidth() / m_TextCols;
	m_NrOfFrames = 2;
	m_NrFramesPerSec = 8;
	m_AnimTime = 0;
	m_AnimFrame = 0;
	m_IsFrozen = false;
	m_Health = 1;
	
	if (playerShape.left < botLeft.x)
	{
		m_Velocity.x = -150;
	}
	else
	{
		m_Velocity.x = 150;
	}
	
}

Zeb::~Zeb()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void Zeb::Update(float elapsedSec, const Level* level, const std::vector<Tile*>& tiles)
{
	m_Life += elapsedSec;

	if (!m_IsFrozen)
	{
		FrameHandling(elapsedSec);
	}

	level->HandleCollision(m_Shape, m_Velocity);

	m_Shape.left += m_Velocity.x * elapsedSec;
	m_Shape.bottom += m_Velocity.y * elapsedSec;

	for (size_t i{}; i < tiles.size(); ++i)
	{
		if (tiles[i])
		{
			tiles[i]->FloorCollision(m_Shape, m_Velocity);
		}
	}
}

void Zeb::Draw() const
{
	if (m_Velocity.x < 0)
	{
		DrawZeb(Point2f{ m_Shape.left, m_Shape.bottom });
	}
	else if (m_Velocity.x > 0)
	{
		glPushMatrix();

		glTranslatef(m_Shape.left, m_Shape.bottom, 0);
		glTranslatef(m_Shape.width, 0, 0);
		glScalef(-1, 1, 1);

		DrawZeb(Point2f{ 0, 0 });

		glPopMatrix();
	}
}

void Zeb::DrawZeb(Point2f pos) const
{
	Rectf srcRect{ 0, m_ClipHeight, m_ClipWidth, m_ClipHeight };

	srcRect.bottom = ((int)m_Type + 1) * m_ClipHeight;
	srcRect.left = m_AnimFrame * m_ClipWidth;

	if (m_IsFrozen)
	{
		srcRect.bottom = 4 * m_ClipHeight;
	}

	m_pTexture->Draw(pos, srcRect);
}

void Zeb::Rise()
{
	++m_Shape.bottom;
}

bool Zeb::GetDead() const
{
	if (m_Life >= m_LifeSpan)
	{
		return true;
	}
	else
	{
		return false;
	}
}
