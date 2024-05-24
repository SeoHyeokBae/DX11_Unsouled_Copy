#include "pch.h"
#include "CBloodScript.h"

CBloodScript::CBloodScript()
	: CScript(BLOODSCRIPT)
	, m_Movement(nullptr)
	, fLife(0.f)
	, bGround(false)
	, vRandDir(Vec2(0.f, 0.f))
	, fMaxLife(0.f)
	, fFlight(0.f)
{
}

CBloodScript::~CBloodScript()
{
}

void CBloodScript::begin()
{
	//fMaxLife = 2.f + (rand() % 10) * 0.15;
	float angle = rand() % 720;
	Vec3 vangle = Vec3(angle, angle, 0.f);
	vangle.ToRadian();
	vRandDir = Vec2(cos(vangle.x), sin(vangle.y));
	vRandDir.Normalize();

	m_Movement = GetOwner()->Movement();
	m_Movement->SetMass(1.f);
	m_Movement->SetVelocity(Vec2(vRandDir.x * 25.f, abs(vRandDir.y * 130.f)));
	m_Movement->SetMaxSpeed(300.f);
	m_Movement->SetFrictionScale(100.f);
	m_Movement->SetGround(false);
}


void CBloodScript::tick()
{
	// 모든 blood lifetime은 동일
	// 생성후 일정시간 뒤에 위로 쏘아줌 소아주는 크기 랜덤
	// 떨어지는 힘 동시 적용
	// 낙하지점 랜덤 (y축 떨어지는 일정속력 도달시 그 지점)
	// 랜덤 텍스처 전달
	// z우선순위 영향받음
	
	m_Movement->AddForce(vRandDir * 25.f);

	if (!m_Movement->IsGround())
	{
		Update_Gravity();
	}
	
	// 바닥 MARK일때 일정 시간뒤 삭제
	// fadeout으로
	if (bGround)
	{
		fLife += DT;

		GetOwner()->Animator2D()->SetCurAnim(nullptr);
		// bloodmark 텍스쳐전달
		// dt만큼 a값조절 fadeout

		if (DT > 3.f)
		{
			Dead();
		}
	}
}

void CBloodScript::Update_Gravity()
{
	// 일정 시간뒤 바닥상태
	fFlight += DT;
	
	if (-125.f > m_Movement->GetVelocity().y)
	{
		m_Movement->SetGround(true);
		m_Movement->SetVelocity(Vec2(0.f,0.f));
	}
	else
	{
		m_Movement->AddForce(Vec2(0.f, -200.f));
	}
}

void CBloodScript::Dead()
{
	GetOwner()->Destroy();
}

void CBloodScript::SaveToFile(FILE* _File)
{
}

void CBloodScript::LoadFromFile(FILE* _File)
{
}

