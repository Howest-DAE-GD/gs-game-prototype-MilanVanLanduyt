#include "pch.h"
#include "SpecialBomb.h"
#include "utils.h"

float SpecialBomb::m_MAXTIME{ 1.f };

SpecialBomb::SpecialBomb(float viewportWidth, float viewportHeigth)
	: m_Radius{ 25.f }
	, m_Transparency{ 1.f }
	, m_Active{ false }
	, m_Destroy{ false }
	, m_Exploded{ false }
	, m_Timer{ 0 }
	, m_Timer2{ 0 }
{
	m_Position.x = rand() % int(viewportWidth - 60.f) + 30.f;
	m_Position.y = rand() % int(viewportHeigth - 90.f) + 30.f;

	m_Circle = Circlef(m_Position, m_Radius);
}

void SpecialBomb::Draw() const
{
	utils::SetColor(Color4f{ 0.3f, 0.2f, 0.2f, m_Transparency });
	if (m_Transparency > 0)
	{
		utils::FillEllipse(m_Position, m_Radius, m_Radius);
	}

	if (m_Active)
	{
		utils::SetColor(Color4f{ 1.f, 1.f, 0.f, 1.f });
		utils::FillEllipse(m_Position, m_Radius / 2.f, m_Radius / 2);
		utils::SetColor(Color4f{ .8f, .8f, .8f, 1.f });
		utils::FillRect(m_Position.x - 2.f, m_Position.y + m_Radius / 2, 4.f, 4.f);

		if (m_Timer2 >= 0.1f && !m_Exploded)
		{
			utils::SetColor(Color4f{ 1.f, 0.f, 0.f, .4f });
			utils::FillRect(m_Position.x - 846.f, m_Position.y - 5.f, 846.f * 2.f, 10.f);
			utils::FillRect(m_Position.x - 5.f, m_Position.y - 500.f, 10.f, 500.f * 2.f);
		}
	}

	if (m_Exploded)
	{
		utils::SetColor(Color4f{ 1.f, 0.f, 0.f, .8f });
		utils::FillRect(m_Position.x - 846.f, m_Position.y - 5.f, 846.f * 2.f, 10.f);
		utils::FillRect(m_Position.x - 5.f, m_Position.y - 500.f, 10.f, 500.f * 2.f);
	}
}

void SpecialBomb::Update(float elapsedSec)
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
		m_Timer2 += elapsedSec;
		if (m_Timer2 >= 0.2f)
		{
			m_Timer2 = 0;
		}

	}

	if (m_Exploded && m_Timer > .2f)
	{
		m_Destroy = true;
	}
}

bool SpecialBomb::CollisionDetection(const Rectf& hitboxPerson)
{
	Rectf hitbox1{
		m_Position.x - 846.f, m_Position.y - 5.f, 846.f * 2.f, 10.f
	},
		hitbox2{
		m_Position.x - 5.f, m_Position.y - 500.f, 10.f, 500.f * 2.f
	};
	if (utils::IsOverlapping(hitboxPerson, m_Circle))
	{
		m_Active = true;
	}

	if ((utils::IsOverlapping(hitboxPerson, hitbox1) || utils::IsOverlapping(hitboxPerson, hitbox2)) && m_Exploded)
	{
		return true;
	}
	else return false;
}

bool SpecialBomb::IsDestroyed() const
{
	return m_Destroy;
}
