#include "pch.h"
#include "CBloodScript.h"

CBloodScript::CBloodScript()
	: CScript(BLOODSCRIPT)
	, m_Movement(nullptr)
	, fLife(0.f)
	, bGround(false)
	, vRandDir(Vec2(0.f, 0.f))
{
}

CBloodScript::~CBloodScript()
{
}



void CBloodScript::begin()
{
	srand(time(0));
	rand();
	
	float angle = rand() % 720;
	vRandDir = Vec2(cos(angle), sin(angle));
	vRandDir.Normalize();

	m_Movement = GetOwner()->Movement();
	m_Movement->SetMass(1.f);
	m_Movement->SetInitSpeed(70.f);

	m_Movement->SetMass(1.f);
	m_Movement->SetInitSpeed(50.f);
	m_Movement->SetMaxSpeed(50.f);
	m_Movement->SetFrictionScale(1000.f);
}

void CBloodScript::tick()
{
	// 모든 blood lifetime은 동일
	// 생성후 일정시간 뒤에 위로 쏘아줌 소아주는 크기 랜덤
	// 떨어지는 힘 동시 적용
	// 낙하지점 랜덤 (y축 떨어지는 일정속력 도달시 그 지점)
	// 랜덤 텍스처 전달
	// z우선순위 영향받음
	
	//Vec2 vVelocity = Vec2(1000.f, 1000.f)* vRandDir;
	m_Movement->AddForce(vRandDir);

	
	// 바닥 MARK일때 일정 시간뒤 삭제
	// fadeout으로
	if (bGround)
	{
		fLife += DT;

		if (DT > 3.f)
		{
			
			Dead();
		}
	}
}

void CBloodScript::Gravity()
{
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

