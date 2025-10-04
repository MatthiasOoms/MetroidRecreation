#include "pch.h"
#include "Quicksand.h"
#include "Texture.h"
#include "Avatar.h"
#include "utils.h"

Quicksand::Quicksand(Point2f pos, int width)
	: m_Shape{ pos.x, pos.y, 16.f, 32 }
	, m_DstWidth{ width }
	, m_pTexture{ new Texture{"Resources/Quicksand.png"} }
{
}

Quicksand::~Quicksand()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void Quicksand::Draw() const
{
	Rectf dstRect{ m_Shape };
	for (int i{}; i < m_DstWidth; ++i)
	{
		dstRect.left += m_Shape.width;
		m_pTexture->Draw(dstRect);
	}
}

void Quicksand::Update(Avatar*& avatar)
{
	Rectf tempRect{ m_Shape };
	for (int i{}; i < m_DstWidth; ++i)
	{
		tempRect.left += m_Shape.width;
		if (utils::IsOverlapping(avatar->GetShape(), tempRect))
		{
			avatar->Damage(2);
		}
	}

}
