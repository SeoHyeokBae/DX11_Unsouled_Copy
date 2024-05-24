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
	// z우선순위 영향받음
	m_Movement->AddForce(vRandDir * 25.f);

	if (!m_Movement->IsGround())
	{
		Update_Gravity();
	}
	
	// 바닥 MARK일때 일정 시간뒤 삭제
	// fadeout으로
	if (m_Movement->IsGround())
	{
		fLife += DT;

		// bloodmark 텍스쳐전달
		// dt만큼 a값조절 fadeout
		GetOwner()->Animator2D()->SetCurAnim(nullptr);
		GetOwner()->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"BloodMark", L"texture\\BloodMark.png"));
		float fidx = rand() % 14;
		float width = CAssetMgr::GetInst()->FindAsset<CTexture>(L"BloodMark")->GetWidth();
		float height = CAssetMgr::GetInst()->FindAsset<CTexture>(L"BloodMark")->GetHeight();
		Vec2 vSize = Vec2(32.f /( float)width, 32.f / (float)height);
		Vec2 vLT = Vec2((vSize.x * fidx, 0.f));
		float fAlpha = 1.f;

		GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_0, vLT); //UV
		GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_1, vSize); //Size
		GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, fAlpha); //알파

		//if (fLife > 4.f)
		//{
		//	Dead();
		//}
	}
}

void CBloodScript::Update_Gravity()
{
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

