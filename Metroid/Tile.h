#pragma once

class Texture;
class Avatar;
struct Vector2f;

class Tile
{
public:
	enum class Type
	{
		blueRubble,
		bubbleBubbles,
		redRubble,
		greenBrick,
		greenRubble
	};
	Tile(Point2f pos, Type tileType);
	Tile(const Tile& other) = delete;
	Tile& operator=(const Tile& other) = delete;
	Tile(Tile&& other) = delete;
	Tile& operator=(Tile&& other) = delete;
	~Tile();

	bool FloorCollision(Rectf& actorShape, Vector2f& actorVelocity);
	bool WallCollision(Rectf& actorShape, Vector2f& actorVelocity);


	Rectf GetShape() const;
	bool IsExploded() const;
	void Draw() const;
	void Update(float elapsedSec);
	void Explode();

private:
	Rectf m_Shape;
	Type m_Type;
	Texture* m_pTexture;
	bool m_IsExploded;

	float m_Rows, m_Cols, 
		m_ClipWidth, m_ClipHeight;
	float m_Frame;

	float m_Timer, m_DeadTimer;
};

