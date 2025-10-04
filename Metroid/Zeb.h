#pragma once
#include "Enemy.h"

class Zeb final : public Enemy
{
public:
	enum class Type
	{
		orange,
		pink,
		red
	};

	Zeb(Point2f botLeft, Rectf playerShape, Type type);
	Zeb(const Zeb& other) = delete;
	Zeb& operator=(const Zeb& other) = delete;
	Zeb(Zeb&& other) = delete;
	Zeb& operator=(Zeb&& other) = delete;
	virtual ~Zeb() final;

	virtual void Update(float elapsedSec, const Level* level, const std::vector<Tile*>& tiles) override final;
	virtual void Draw() const override final;
	void DrawZeb(Point2f pos) const;
	void Rise();
	bool GetDead() const;

private:
	Type m_Type;
	int m_LifeSpan;
	float m_Life;
};

