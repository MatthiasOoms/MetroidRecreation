#pragma once
#include "pch.h"
#include "PowerUp.h"
#include "Texture.h"
#include "Avatar.h"
#include "utils.h"

PowerUp::PowerUp(Type type, Point2f pos)
	: m_Type{ type }
	, m_Shape{ pos.x, pos.y, 16, 16 }
	, m_pTexture{ new Texture("Resources/PowerUpList.png") }
	, m_Rows{ 7 }
	, m_Cols{ 1 }
	, m_ClipHeight{ m_pTexture->GetHeight() / m_Rows }
	, m_ClipWidth{ m_pTexture->GetWidth() / m_Cols }
{
}

PowerUp::~PowerUp()
{
	if (m_pTexture != nullptr)
	{
		delete m_pTexture;
		m_pTexture = nullptr;
	}
}

void PowerUp::Draw() const
{
	if (m_pTexture != nullptr)
	{
		Rectf srcRect{0, m_ClipHeight, m_ClipWidth, m_ClipHeight };

		srcRect.bottom += m_ClipHeight * (int)m_Type;

		m_pTexture->Draw(m_Shape, srcRect);
	}
}

void PowerUp::Update(Avatar* player)
{
	if (utils::IsOverlapping(m_Shape, player->GetShape()))
	{
		if (m_pTexture != nullptr)
		{
			delete m_pTexture;
			m_pTexture = nullptr;

			player->PowerUpHit(m_Type);
		}
	}
}
