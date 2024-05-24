#include "pch.h"
#include "CMovement.h"

#include "CTimeMgr.h"
#include "CTransform.h"

// �밢�� �̵� �����ʿ�

CMovement::CMovement()
	: CComponent(COMPONENT_TYPE::MOVEMENT)
	, m_Mass(1.f)
	, m_InitSpeed(0.f)
	, m_MaxSpeed(0.f)
	, m_FrictionScale(0.f)
	, m_Ground(true)
{
}

CMovement::~CMovement()
{
}

void CMovement::finaltick()
{
	m_Accel = m_Force / m_Mass;

	// ������ ����� ������ ���
	if (m_Velocity.Length() < 0.1f)
	{
		if (!m_Accel.IsZero())
		{
			Vec2 vAccelDir = m_Accel;
			vAccelDir.Normalize();
			m_Velocity = vAccelDir * m_InitSpeed;
		}
	}
	else
	{
		m_Velocity += m_Accel * DT;
	}

	// �ִ� �ӵ� ����
	if (fabs(m_Velocity.x) > m_MaxSpeed)
	{
		m_Velocity.x = (m_Velocity.x / abs(m_Velocity.x)) * m_MaxSpeed;
	}
	if (fabs(m_Velocity.y) > m_MaxSpeed)
	{
		m_Velocity.y = (m_Velocity.y / abs(m_Velocity.y)) * m_MaxSpeed;
	}

	if (m_Force.IsZero() && m_Ground && (m_Velocity.x != 0.f || m_Velocity.y != 0.f))
	{
		// X ��ǥ�� ���� ������ ���
		if (m_Velocity.x != 0.f)
		{
			float xFriction = -m_Velocity.x;
			xFriction /= abs(xFriction);
			xFriction *= m_FrictionScale;

			float xFrictionAccel = (xFriction / m_Mass) * DT;
			if (abs(m_Velocity.x) < abs(xFrictionAccel))
			{
				m_Velocity.x = 0.f;
			}
			else
			{
				m_Velocity.x += xFrictionAccel;
			}
		}

		// Y ��ǥ�� ���� ������ ���
		if (m_Velocity.y != 0.f)
		{
			float yFriction = -m_Velocity.y;
			yFriction /= abs(yFriction);
			yFriction *= m_FrictionScale;

			float yFrictionAccel = (yFriction / m_Mass) * DT;
			if (abs(m_Velocity.y) < abs(yFrictionAccel))
			{
				m_Velocity.y = 0.f;
			}
			else
			{
				m_Velocity.y += yFrictionAccel;
			}
		}
	}


	Vec3 vObjPos = Transform()->GetRelativePos();
	vObjPos = Vec3(vObjPos.x + m_Velocity.x * DT, vObjPos.y + m_Velocity.y * DT, vObjPos.z);
	GetOwner()->Transform()->SetRelativePos(vObjPos);

	// �� ����
	m_Force = Vec2(0.f, 0.f);
}

void CMovement::SaveToFile(FILE* _File)
{
	 fwrite(&m_Mass, sizeof(float), 1, _File);
	 fwrite(&m_InitSpeed, sizeof(float), 1, _File);
	 fwrite(&m_MaxSpeed, sizeof(float), 1, _File);
	 fwrite(&m_FrictionScale, sizeof(float), 1, _File);
	 fwrite(&m_Ground, sizeof(bool), 1, _File);
}

void CMovement::LoadFromFile(FILE* _File)
{
	fread(&m_Mass, sizeof(float), 1, _File);
	fread(&m_InitSpeed, sizeof(float), 1, _File);
	fread(&m_MaxSpeed, sizeof(float), 1, _File);
	fread(&m_FrictionScale, sizeof(float), 1, _File);
	fread(&m_Ground, sizeof(bool), 1, _File);
}

// ��ü �ʱ�ӵ� �ٽû���
// �� �������� �ִ����� ������ ���x ���̾����� ���������� ������� ������x
// 
//�ִ������������� ����� ���� 
//if (!(_mVelocity == Vector2::Zero))
//{
//	// �ӵ��� �ݴ� �������� ������ ����
//	Vector2 frictionDir = -_mVelocity;
//	friction = frictionDir.normalize() * _mFriction * _mMass * TimeMgr::DeltaTime();
//
//	// ���������� ���� �ӵ� ���ҷ��� ���� �ӵ����� ū ���
//	if (_mVelocity.length() < friction.length())
//	{
//		// ����
//		_mVelocity = Vector2(0.f, 0.f);
//	}
//	else
//	{
//		// ���������� �ӵ� ����
//		_mVelocity += friction;
//	}
//}



	// ��ü�� ����ǰ��ִ� ���� ������ �������� �����Ų��.
	//if (m_Force.IsZero() && m_Velocity.x != 0.f && m_Ground)
	//{
	//	float fFriction = -m_Velocity.x;
	//	fFriction /= abs(fFriction);

	//	fFriction *= m_FrictionScale;

	//	float fFrictionAccel = (fFriction / m_Mass) * DT;
	//	if (abs(m_Velocity.x) < abs(fFrictionAccel))
	//	{
	//		m_Velocity = Vec2(0.f, m_Velocity.y);
	//	}
	//	else
	//	{
	//		m_Velocity.x += fFrictionAccel;
	//	}
	//}
	//if (m_Force.IsZero() && m_Velocity.y != 0.f && m_Ground)
	//{
	//	float fFriction = -m_Velocity.y;
	//	fFriction /= abs(fFriction);

	//	fFriction *= m_FrictionScale;

	//	float fFrictionAccel = (fFriction / m_Mass) * DT;
	//	if (abs(m_Velocity.y) < abs(fFrictionAccel))
	//	{
	//		m_Velocity = Vec2(m_Velocity.x,0.f);
	//	}
	//	else
	//	{
	//		m_Velocity.y += fFrictionAccel;
	//	}
	//}