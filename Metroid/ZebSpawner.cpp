#include "pch.h"
#include "ZebSpawner.h"
#include "Zeb.h"
#include "Avatar.h"
#include "Level.h"
#include "Tile.h"
#include "Texture.h"
#include "utils.h"

ZebSpawner::ZebSpawner(Point2f botLeft)
	: m_Shape{ botLeft.x, botLeft.y, 32, 16 }
	, m_pZeb{}
	, m_AccuSec{ 0 }
	, m_Cooldown{ 1 }
	, m_pTexture{ new Texture{"Resources/ZebSpawner.png"}}
{
}

ZebSpawner::~ZebSpawner()
{
	if (m_pZeb)
	{
		delete m_pZeb;
		m_pZeb = nullptr;
	}
	delete m_pTexture;
	m_pTexture = nullptr;
}

Zeb*& ZebSpawner::GetZeb()
{
	return m_pZeb;
}

void ZebSpawner::Update(float elapsedSec, Avatar*& player, const Level* level, const std::vector<Tile*>& tiles)
{
	m_AccuSec += elapsedSec;
	if (m_AccuSec >= m_Cooldown)
	{
		if (!m_pZeb)
		{
			m_AccuSec = 0;
			// m_Shape.left + m_Shape.width / 2 - 8		(The -8 is half the width of the Zeb)
			m_pZeb = new Zeb{ Point2f(m_Shape.left + m_Shape.width / 2 - 8, m_Shape.bottom + m_Shape.height), player->GetShape(), Zeb::Type::orange };
		}
	}

	if (m_pZeb)
	{
		if (!GetZeb()->IsFrozen())
		{
			if (m_pZeb->GetShape().bottom > m_Shape.bottom + m_Shape.height * 2)
			{
				m_pZeb->Update(elapsedSec, level, tiles);
			}
			else
			{
				m_pZeb->Rise();
			}

			if (utils::IsOverlapping(m_pZeb->GetShape(), player->GetShape()))
			{
				player->Damage(3);
			}

			if (m_pZeb->GetDead())
			{
				delete m_pZeb;
				m_pZeb = nullptr;
			}
		}
		else
		{
		}
	}
	
}

void ZebSpawner::Draw() const
{
	m_pTexture->Draw(m_Shape);
	if (m_pZeb)
	{
		m_pZeb->Draw();
	}
}
