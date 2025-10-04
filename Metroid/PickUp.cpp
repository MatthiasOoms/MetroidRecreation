#pragma once
#include "pch.h"
#include "PickUp.h"
#include "Texture.h"
#include "Avatar.h"
#include "utils.h"

PickUp::PickUp(Point2f pos, bool hasMissiles)
	: m_Shape{ pos.x, pos.y, 8, 8 }
	, m_pTexture{ new Texture("Resources/PickUpList.png") }
	, m_Rows{ 3 }
	, m_Cols{ 2 }
	, m_Frame{}
	, m_FramesPerSec{ 6 }
	, m_AccuSec{}
	, m_ClipHeight{ m_pTexture->GetHeight() / m_Rows }
	, m_ClipWidth{ m_pTexture->GetWidth() / m_Cols }
{
	int chance{ ((rand() % 100) + 1) };
	if (chance <= 2)
	{
		m_Type = Type::nothing;
	}
	else if (chance <= 26)
	{
		m_Type = Type::energySmall;
	}
	else if (chance <= 50)
	{
		m_Type = Type::energyLarge;
	}
	else if (chance <= 74 && hasMissiles)
	{
		m_Type = Type::missile;
	}
	else
	{
		m_Type = Type::nothing;
	}
}

PickUp::~PickUp()
{
	if (m_pTexture != nullptr)
	{
		delete m_pTexture;
		m_pTexture = nullptr;
	}
}

void PickUp::Draw() const
{
	if (m_pTexture != nullptr)
	{
		Rectf srcRect{ 0, 0, m_ClipWidth, m_ClipHeight };

		srcRect.left = (m_Frame / 2) * m_ClipWidth;

		switch (m_Type)
		{
		case PickUp::Type::nothing:
			break;
		case PickUp::Type::energySmall:
			srcRect.bottom = m_ClipHeight;
			break;
		case PickUp::Type::energyLarge:
			srcRect.bottom = m_ClipHeight;
			break;
		case PickUp::Type::missile:
			srcRect.bottom = 0;
			srcRect.height = m_ClipHeight * 2;
			break;
		default:
			break;
		}

		m_pTexture->Draw(Point2f(m_Shape.left, m_Shape.bottom), srcRect);
	}
}

void PickUp::Update(Avatar* player, float elapsedSec)
{
	if (utils::IsOverlapping(m_Shape, player->GetShape()))
	{
		if (m_pTexture != nullptr)
		{
			delete m_pTexture;
			m_pTexture = nullptr;

			player->PickUpHit(m_Type);
		}
	}

	m_AccuSec += m_FramesPerSec * elapsedSec;
	m_Frame = (int)m_AccuSec % m_Cols + 1;
}

PickUp::Type PickUp::GetType() const
{
	return m_Type;
}
