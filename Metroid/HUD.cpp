#include "pch.h"
#include "HUD.h"
#include "Texture.h"

HUD::HUD()
	: m_Energy{ 0 }
	, m_Missiles{ 0 }
	, m_pHUDTexture{ new Texture{"Resources/HUD.png"} }
	, m_pFontTexture{ new Texture{"Resources/Font.png"} }
	, m_Cols{ 3 }
	, m_Rows{ 3 }
	, m_FontCols{ 10 }
	, m_FontRows{ 1 }
	, m_Packs{ 0 }
	, m_HasMissiles{ false }
{
}

HUD::~HUD()
{
	delete m_pHUDTexture;
	m_pHUDTexture = nullptr;

	delete m_pFontTexture;
	m_pFontTexture = nullptr;
}

void HUD::Update(int energy, int packs, int missiles, bool hasMissiles)
{
	SetEnergy(energy);
	SetPacks(packs);
	SetMissiles(missiles, hasMissiles);
}

void HUD::DrawLarge(Point2f& location) const
{
	Rectf srcRect{};
	Point2f dest{};

	srcRect.height = m_pHUDTexture->GetHeight() / m_Rows;
	srcRect.width = m_pHUDTexture->GetWidth() / m_Cols;


	for (int i{}; i < m_Rows - 1; ++i) // - 1 since 1 small row
	{
		for (int j{}; j < m_Cols; ++j) 
		{
			srcRect.bottom = srcRect.height - (m_Cols - i) * srcRect.height;
			srcRect.left = j * srcRect.width;

			dest.x = location.x + srcRect.width * j;
			dest.y = location.y - srcRect.height * i;

			if (i == 1 && m_HasMissiles)
			{
				m_pHUDTexture->Draw(dest, srcRect);
			}
			else if (i != 1)
			{
				m_pHUDTexture->Draw(dest, srcRect);
			}
		}
	}
}

void HUD::DrawSmall(Point2f& location) const
{
	Rectf srcRect{};
	Point2f dest{};
	float offset{ 2 };

	srcRect.height = m_pHUDTexture->GetHeight() / m_Rows;
	srcRect.width = m_pHUDTexture->GetWidth() / m_Cols;

	for (int i{}; i < m_Packs; ++i)
	{
		srcRect.left = srcRect.width;
		if ((m_Energy / ((i + 1) * 100)) < 1)
		{
			srcRect.left = 0;
		}

		dest.x = ((location.x) + 5 * srcRect.width) - i * srcRect.width - i * offset;
		dest.y = (location.y) + srcRect.height;

		m_pHUDTexture->Draw(dest, srcRect);
	}
	location.x += m_Cols * srcRect.width; 
}

void HUD::DrawAmounts(Point2f& location) const
{
	Rectf srcRect{};
	Point2f dstPoint{};

	srcRect.width = m_pFontTexture->GetWidth() / m_FontCols;
	srcRect.height = m_pFontTexture->GetHeight() / m_FontRows;
	
	// 2 Energy digits
	int drawDigits{ 2 };
	dstPoint.y = location.y;

	for (int i{}; i < drawDigits; ++i)
	{
		// digits - 1 - i because there are already 2 elements drawn
		dstPoint.x = location.x + (drawDigits - 1 - i) * (srcRect.width);

		if (i > 0)
		{
			srcRect.left = (m_Energy / (i * 10)) * srcRect.width;
		}
		else
		{
			srcRect.left = ((m_Energy - (m_Energy / 10) * 10)) * srcRect.width;
		}
		m_pFontTexture->Draw(dstPoint, srcRect);
	}
	// Move left 1 digit because Missiles has 3 digits
	location.x -= srcRect.width;

	if (m_HasMissiles)
	{
		// 3 Missile digits
		drawDigits = 3;

		int digit{ m_Missiles - ((m_Missiles / 100) * 100) - (((m_Missiles / 10) - 10 * (m_Missiles / 100)) * 10) };
		int decimals{ m_Missiles / 10 - ((m_Missiles / 100) * 10) };
		int hundreds{ (m_Missiles / 100) };

		dstPoint.y -= srcRect.height;
		for (int i{}; i < drawDigits; ++i)
		{
			// digits - 2 - i because you start 2 from the given position
			dstPoint.x = location.x + (drawDigits - 1 - i) * (srcRect.width);

			switch (i)
			{
			case 0:
				srcRect.left = digit * srcRect.width;
				break;
			case 1:
				srcRect.left = decimals * srcRect.width;
				break;
			case 2:
				srcRect.left = hundreds * srcRect.width;
				break;
			}
			m_pFontTexture->Draw(dstPoint, srcRect);
		}
	}
}

void HUD::SetEnergy(int energy)
{
	m_Energy = energy;
}

void HUD::SetPacks(int packs)
{
	m_Packs = packs;
}

void HUD::SetMissiles(int missiles, bool hasMissiles)
{
	m_Missiles = missiles;
	m_HasMissiles = hasMissiles;
}
