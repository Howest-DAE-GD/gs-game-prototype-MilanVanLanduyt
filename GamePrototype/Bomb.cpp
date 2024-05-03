#include "pch.h"
#include "Bomb.h"
#include "utils.h"

float Bomb::m_MAXTIME{ 1.f };

Bomb::Bomb(float viewportWidth, float viewportHeigth)
	: m_Radius{ 25.f }
	, m_Transparency{ 1.f }
	, m_Active{ false }
	, m_Destroy{ false }
	, m_Exploded{ false }
	, m_Timer{ 0 }
{
	m_Position.x = rand() % int(viewportWidth - 60.f) + 30.f;
	m_Position.y = rand() % int(viewportHeigth - 90.f) + 30.f;

	m_Circle = Circlef(m_Position, m_Radius);
}

void Bomb::Draw() const
{
	utils::SetColor(Color4f{ 0.3f, 0.2f, 0.2f, m_Transparency });
	if (m_Transparency > 0)
	{
		utils::FillEllipse(m_Position, m_Radius, m_Radius);
	}

	if (m_Active)
	{
		utils::SetColor(Color4f{ 0.f, 0.f, 0.f, 1.f });
		utils::FillEllipse(m_Position, m_Radius / 2.f, m_Radius / 2);
		utils::SetColor(Color4f{ .8f, .8f, .8f, 1.f });
		utils::FillRect(m_Position.x - 2.f, m_Position.y + m_Radius / 2, 4.f, 4.f);
	}

	if (m_Exploded)
	{
		utils::SetColor(Color4f{ 1.f, 0.f, 0.f, .8f });
		utils::FillEllipse(m_Position, m_Radius, m_Radius);
	}
}

void Bomb::Update(float elapsedSec)
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

		if (m_Timer >= m_MAXTIME)
		{
			m_Timer = 0;
			m_Exploded = true;
		}
	}

	if (m_Exploded && m_Timer > .5f)
	{
		m_Destroy = true;
	}
}

bool Bomb::CollisionDetection(const Rectf& hitboxPerson)
{
	if (utils::IsOverlapping(hitboxPerson, m_Circle) && m_Exploded)
	{
		m_Active = true;
		return true;
	}
	else if (utils::IsOverlapping(hitboxPerson, m_Circle))
	{
		m_Active = true;
		return false;
	}
	else return false;

	
}

bool Bomb::IsDestroyed() const
{
	return m_Destroy;
}
