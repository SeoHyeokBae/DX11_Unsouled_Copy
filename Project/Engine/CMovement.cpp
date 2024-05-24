#include "pch.h"
#include "CMovement.h"

#include "CTimeMgr.h"
#include "CTransform.h"

// 대각선 이동 수정필요

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

	// 정지에 가까운 상태일 경우
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

	// 최대 속도 제한
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
		// X 좌표에 대한 마찰력 계산
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

		// Y 좌표에 대한 마찰력 계산
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

	// 힘 리셋
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

// 물체 초기속도 다시생각
// 위 마찰력은 최대정지 마찰력 고려x 힘이없을때 마찰력적용 힘적용시 마찰력x
// 
//최대정지마찰력을 고려한 수식 
//if (!(_mVelocity == Vector2::Zero))
//{
//	// 속도에 반대 방향으로 마찰력 적용
//	Vector2 frictionDir = -_mVelocity;
//	friction = frictionDir.normalize() * _mFriction * _mMass * TimeMgr::DeltaTime();
//
//	// 마찰력으로 의한 속도 감소량이 현재 속도보다 큰 경우
//	if (_mVelocity.length() < friction.length())
//	{
//		// 멈춤
//		_mVelocity = Vector2(0.f, 0.f);
//	}
//	else
//	{
//		// 마찰력으로 속도 차감
//		_mVelocity += friction;
//	}
//}



	// 물체에 적용되고있는 힘이 없으면 마찰력을 적용시킨다.
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