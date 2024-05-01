#pragma once
class Bones
{
public:
	explicit Bones(float viewportWidth, float viewportHeigth);
	~Bones();

	void Draw() const;
	void Update(float elapsedSec);
	bool CollisionDetection(const Rectf& hitboxPerson);
	bool IsDestroyed() const;
	void DestroyBone();
private:
	static float m_MAXTIME;

	Point2f m_Position;
	Circlef m_Circle;
	float m_Timer;
	float m_Radius;
	float m_Transparency;
	bool m_Active;
	bool m_Destroy;
};

