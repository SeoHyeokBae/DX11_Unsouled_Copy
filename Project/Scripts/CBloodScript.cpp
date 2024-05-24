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
	// ��� blood lifetime�� ����
	// ������ �����ð� �ڿ� ���� ����� �Ҿ��ִ� ũ�� ����
	// �������� �� ���� ����
	// �������� ���� (y�� �������� �����ӷ� ���޽� �� ����)
	// ���� �ؽ�ó ����
	// z�켱���� �������
	
	m_Movement->AddForce(vRandDir * 25.f);

	if (!m_Movement->IsGround())
	{
		Update_Gravity();
	}
	
	// �ٴ� MARK�϶� ���� �ð��� ����
	// fadeout����
	if (bGround)
	{
		fLife += DT;

		GetOwner()->Animator2D()->SetCurAnim(nullptr);
		// bloodmark �ؽ�������
		// dt��ŭ a������ fadeout

		if (DT > 3.f)
		{
			Dead();
		}
	}
}

void CBloodScript::Update_Gravity()
{
	// ���� �ð��� �ٴڻ���
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

