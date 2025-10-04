#pragma once

class Avatar;

class Room
{
public:
	Room(Point2f pos, int roomWidth, int roomHeight);
	Room(const Room& other) = delete;
	Room& operator=(const Room& other) = delete;
	Room(Room&& other) = delete;
	Room& operator=(Room&& other) = delete;
	~Room();

	Rectf GetShape() const;
	bool GetActive() const;
	void Update(Avatar* player);

private:
	Rectf m_Shape;
	bool m_IsActive;
};

