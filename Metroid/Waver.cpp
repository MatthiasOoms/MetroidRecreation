#include "pch.h"
#include "Waver.h"
#include "Level.h"
#include "Texture.h"
#include "Tile.h"
#include "utils.h"

Waver::Waver(Point2f botLeft, Type type)
	: Enemy()
	, m_Type{ type }
	, m_Increment{ 2 }
{
	m_Shape = Rectf(botLeft.x, botLeft.y, 16, 8);
	m_pTexture = new Texture("Resources/Waver.png");
	m_TextRows = 3;
	m_TextCols = 3;
	m_ClipHeight = m_pTexture->GetHeight() / m_TextRows;
	m_ClipWidth = m_pTexture->GetWidth() / m_TextCols;
	m_NrOfFrames = 3;
	m_NrFramesPerSec = 3;
	m_AnimTime = 0;
	m_AnimFrame = 0;
	m_Velocity.x = 100;
	m_Velocity.y = 50;
}

Waver::~Waver()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void Waver::Update(float elapsedSec, const Level* level, const std::vector<Tile*>& tiles)
{
	if (m_Velocity.y > 100)
	{
		m_Increment = -2;
	}
	else if (m_Velocity.y < -100)
	{
		m_Increment = 2;
	}

	m_Velocity.y += m_Increment;

	if (!m_IsFrozen)
	{
		FrameHandling(elapsedSec);
	}

	level->HandleRipperCollision(m_Shape, m_Velocity);

	m_Shape.left += m_Velocity.x * elapsedSec;
	m_Shape.bottom += m_Velocity.y * elapsedSec;

	for (size_t i{}; i < tiles.size(); ++i)
	{
		if (tiles[i])
		{
			if (!tiles[i]->IsExploded())
			{
				if (tiles[i]->WallCollision(m_Shape, m_Velocity))
				{
					m_Velocity.x *= -1;
				}
			}
		}
	}
}

void Waver::DrawWaver(Point2f pos) const
{
	Rectf srcRect{ 0, m_ClipHeight, m_ClipWidth, m_ClipHeight };

	srcRect.bottom = ((int)m_Type + 1) * m_ClipHeight;
	if (IsFrozen())
	{
		srcRect.bottom = 0;
	}
	srcRect.left = m_AnimFrame * m_ClipWidth;

	m_pTexture->Draw(pos, srcRect);
}

void Waver::Draw() const
{
	// Check velocity direction
	if (m_Velocity.x > 0)
	{
		DrawWaver(Point2f{ m_Shape.left, m_Shape.bottom });
	}
	else if (m_Velocity.x < 0)
	{
		glPushMatrix();

		glTranslatef(m_Shape.left, m_Shape.bottom, 0);
		glTranslatef(m_Shape.width, 0, 0);
		glScalef(-1, 1, 1);

		DrawWaver(Point2f{ 0, 0 });

		glPopMatrix();
	}
}
