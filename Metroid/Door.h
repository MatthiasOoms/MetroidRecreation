#pragma once

class Texture;

class Door
{
public:
	enum class Type
	{
		blue,
		red
	};
	Door(Point2f pos, Type doorType);
	Door(const Door& other) = delete;
	Door& operator=(const Door& other) = delete;
	Door(Door&& other) = delete;
	Door& operator=(Door&& other) = delete;
	~Door();

	void Update(float elapsedSec);
	void Draw() const;
	Rectf GetShape() const;
	bool IsOpen() const;
	Type GetType() const;
	void Open();

private:
	Rectf m_Shape;
	Texture* m_pTexture;
	Type m_Type;

	float m_Rows, m_Cols, m_ClipHeight, m_ClipWidth;
	bool m_IsOpen;
	int m_Frame;
	float m_Timer, m_OpenDuration;
};

