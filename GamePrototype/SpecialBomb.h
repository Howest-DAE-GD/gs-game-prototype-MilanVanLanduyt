#pragma once
class SpecialBomb
{
public:
	explicit SpecialBomb(float viewportWidth, float viewportHeigth);

	void Draw() const;
	void Update(float elapsedSec);
	bool CollisionDetection(const Rectf& hitboxPerson);
	bool IsDestroyed() const;
private:
	static float m_MAXTIME;

	Point2f m_Position;
	Circlef m_Circle;
	float m_Timer;
	float m_Timer2;
	float m_Radius;
	float m_Transparency;
	bool m_Active;
	bool m_Destroy;
	bool m_Exploded;
};

