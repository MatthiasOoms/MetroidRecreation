#pragma once

class Camera
{
public:
	Camera(float width, float height);
	Camera(const Camera& other) = delete;
	Camera& operator=(const Camera& other) = delete;
	Camera(Camera&& other) = delete;
	Camera& operator=(Camera&& other) = delete;
	~Camera();

	void SetLevelBoundaries(const Rectf& levelBoundaries);
	void Transform(const Rectf& target);

private:
	const float m_Width;
	const float m_Height;
	Rectf m_LevelBoundaries;

	Point2f Track(const Rectf& target) const;
	void Clamp(Point2f& bottomLeftPos);
};

