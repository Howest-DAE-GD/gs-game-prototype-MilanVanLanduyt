#include "pch.h"
#include "Bones.h"
#include "utils.h"

float Bones::m_MAXTIME{ 4.f };

Bones::Bones(float viewportWidth, float viewportHeigth)
	: m_Radius{ 25.f }
	, m_Transparency{ 1.f }
	, m_Active{ false }
	, m_Destroy{ false }
	, m_Timer{ 0 }
{
	m_Position.x = rand() % int(viewportWidth - 60.f) + 30.f;
	m_Position.y = rand() % int(viewportHeigth - 90.f) + 30.f;

	m_Circle = Circlef(m_Position, m_Radius);
}

Bones::~Bones()
{

}

void Bones::Draw() const
{
	utils::SetColor(Color4f{ 0.3f, 0.2f, 0.2f, m_Transparency });
	if (m_Transparency > 0)
	{
		utils::FillEllipse(m_Position, m_Radius, m_Radius);
	}

	if (m_Active)
	{
		utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });
		utils::FillRect(m_Position.x - 7.5f, m_Position.y - 3.f, 15.5f, 6.f);
		utils::FillEllipse(m_Position.x - 12.f, m_Position.y + 3.f, 5.f, 5.f);
		utils::FillEllipse(m_Position.x + 12.f, m_Position.y + 3.f, 5.f, 5.f);

		utils::FillEllipse(m_Position.x - 12.f, m_Position.y - 3.f, 5.f, 5.f);
		utils::FillEllipse(m_Position.x + 12.f, m_Position.y - 3.f, 5.f, 5.f);
	}
}

void Bones::Update(float elapsedSec)
{
	if (m_Transparency >= 0 && !m_Active)
	{
		m_Transparency -= 0.1f * elapsedSec;
	}
	else if (m_Transparency < 0)
	{
		m_Destroy = true;
	}

	if (m_Active)
	{
		m_Timer += elapsedSec;
	}

	if (m_Timer >= m_MAXTIME)
	{
		m_Destroy = true;
	}
}

bool Bones::CollisionDetection(const Rectf& hitboxPerson)
{
	if (utils::IsOverlapping(hitboxPerson, m_Circle) && m_Active)
	{
		return true;
	}
	else if (utils::IsOverlapping(hitboxPerson, m_Circle) && !m_Active)
	{
		m_Active = true;
		return false;
	}
	else return false;
}

bool Bones::IsDestroyed() const
{
	return m_Destroy;
}

void Bones::DestroyBone()
{
	m_Destroy = true;
}
