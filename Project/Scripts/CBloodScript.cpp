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
	// ��� blood lifetime�� ����
	// ������ �����ð� �ڿ� ���� ����� �Ҿ��ִ� ũ�� ����
	// �������� �� ���� ����
	// �������� ���� (y�� �������� �����ӷ� ���޽� �� ����)
	// ���� �ؽ�ó ����
	// z�켱���� �������
	
	//Vec2 vVelocity = Vec2(1000.f, 1000.f)* vRandDir;
	m_Movement->AddForce(vRandDir);

	
	// �ٴ� MARK�϶� ���� �ð��� ����
	// fadeout����
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

