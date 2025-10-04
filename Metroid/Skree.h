#pragma once
#include "Enemy.h"

class SkreeBall;

class Skree final : public Enemy
{
public:
	enum class Type
	{
		green,
		darkGreen
	};

	Skree(Point2f botLeft, Type type);
	Skree(const Skree& other) = delete;
	Skree& operator=(const Skree& other) = delete;
	Skree(Skree&& other) = delete;
	Skree& operator=(Skree&& other) = delete;
	virtual ~Skree() override final;

	void Update(float elapsedSec, const Level* level, const std::vector<Tile*>& tiles) override final;
	void Draw() const override final;
	bool GetDeath() const;
	void Attack(Rectf actorShape);
	std::vector<SkreeBall*>& GetSkreeBalls();
private:
	std::vector<SkreeBall*> m_pProjectiles;
	Type m_Type;
	bool m_IsDead;
	float m_Life;
	int m_LifeSpan;
	bool m_HasShot;
	bool m_IsAttacking;

	void DrawSkree(Point2f pos) const;
	void DrawSkreeHalf(Point2f pos) const;
	void Death();
	void Explode(float elapsedSec);

};

