#include "pch.h"
#include "Zoomer.h"
#include "Level.h"
#include "Texture.h"
#include "Tile.h"
#include "utils.h"

Zoomer::Zoomer(Point2f botLeft, Type zoomerType, Direction zoomerDirection)
	: Enemy()
	, m_Type { zoomerType }
	, m_Direction{ zoomerDirection }
{
	m_Shape = Rectf(botLeft.x, botLeft.y, 16, 16);
	m_pTexture = new Texture("Resources/Zoomer.png");
	m_TextRows = 4;
	m_TextCols = 2;
	m_ClipHeight = m_pTexture->GetHeight() / m_TextRows;
	m_ClipWidth = m_pTexture->GetWidth() / m_TextCols;
	m_NrOfFrames = 2;
	m_NrFramesPerSec = 8;
	m_AnimTime = 0;
	m_AnimFrame = 0;
}

Zoomer::~Zoomer()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void Zoomer::Update(float elapsedSec, const Level* level, const std::vector<Tile*>& tiles)
{
	if (!m_IsFrozen)
	{
		FrameHandling(elapsedSec);
	}

	level->HandleZoomerCollision(m_Shape, m_Velocity, m_Direction);

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

void Zoomer::Draw() const
{
	if (m_Direction == Direction::clockwise)
	{
		ClockwiseDraw();
	}
	else
	{
		CounterClockwiseDraw();
	}
}

void Zoomer::DrawZoomer(Point2f pos) const
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

void Zoomer::ClockwiseDraw() const
{
	// Check velocity direction
	if (m_Velocity.x > 0)
	{
		DrawZoomer(Point2f{ m_Shape.left, m_Shape.bottom });
	}
	else if (m_Velocity.x < 0)
	{
		glPushMatrix();

		glTranslatef(m_Shape.left, m_Shape.bottom, 0);
		glTranslatef(m_Shape.width, m_Shape.height, 0);
		glScalef(-1, 1, 1);
		glRotatef(180, 1, 0, 0);

		DrawZoomer(Point2f{ 0, 0 });

		glPopMatrix();
	}
	else if (m_Velocity.y > 0)
	{
		glPushMatrix();

		glTranslatef(m_Shape.left, m_Shape.bottom, 0);
		glTranslatef(m_Shape.width, 0, 0);
		glScalef(-1, 1, 1);
		glRotatef(-180, 1, 1, 0);

		DrawZoomer(Point2f{ 0, 0 });

		glPopMatrix();
	}
	else if (m_Velocity.y < 0)
	{
		glPushMatrix();

		glTranslatef(m_Shape.left, m_Shape.bottom, 0);
		glRotatef(180, 1, 1, 0);

		DrawZoomer(Point2f{ 0, 0 });

		glPopMatrix();
	}
}

void Zoomer::CounterClockwiseDraw() const
{
	// Check velocity direction
	if (m_Velocity.x < 0)
	{
		DrawZoomer(Point2f{ m_Shape.left, m_Shape.bottom });
	}
	else if (m_Velocity.x > 0)
	{
		glPushMatrix();

		glTranslatef(m_Shape.left, m_Shape.bottom, 0);
		glTranslatef(m_Shape.width, m_Shape.height, 0);
		glScalef(-1, 1, 1);
		glRotatef(180, 1, 0, 0);

		DrawZoomer(Point2f{ 0, 0 });

		glPopMatrix();
	}
	else if (m_Velocity.y < 0)
	{
		glPushMatrix();

		glTranslatef(m_Shape.left, m_Shape.bottom, 0);
		glTranslatef(m_Shape.width, 0, 0);
		glScalef(-1, 1, 1);
		glRotatef(-180, 1, 1, 0);

		DrawZoomer(Point2f{ 0, 0 });

		glPopMatrix();
	}
	else if (m_Velocity.y > 0)
	{
		glPushMatrix();

		glTranslatef(m_Shape.left, m_Shape.bottom, 0);
		glRotatef(180, 1, 1, 0);

		DrawZoomer(Point2f{ 0, 0 });

		glPopMatrix();
	}
}
