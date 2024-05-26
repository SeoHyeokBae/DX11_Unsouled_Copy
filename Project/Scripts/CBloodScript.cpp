#include "pch.h"
#include "CBloodScript.h"

CBloodScript::CBloodScript()
	: CScript(BLOODSCRIPT)
	, m_Movement(nullptr)
	, m_fLife(0.f)
	, m_fAlpha(1.f)
	, m_fidx(0.f)
	, m_bGround(false)
	, m_vRandDir(Vec2(0.f, 0.f))
{
}

CBloodScript::~CBloodScript()
{
}

void CBloodScript::begin()
{
	m_fidx = rand() % 14;
	float angle = rand() % 720;
	Vec3 vangle = Vec3(angle, angle, 0.f);
	vangle.ToRadian();
	m_vRandDir = Vec2(cos(vangle.x), sin(vangle.y));
	m_vRandDir.Normalize();

	m_Movement = GetOwner()->Movement();
	m_Movement->SetMass(1.f);
	m_Movement->SetVelocity(Vec2(m_vRandDir.x * 25.f, abs(m_vRandDir.y * 130.f)));
	m_Movement->SetMaxSpeed(300.f);
	m_Movement->SetFrictionScale(100.f);
	m_Movement->SetGround(false);
}


void CBloodScript::tick()
{
	// z우선순위 영향받음
	m_Movement->AddForce(m_vRandDir * 25.f);

	if (!m_Movement->IsGround())
	{
		Update_Gravity();
	}
	
	if (m_Movement->IsGround())
	{
		m_fLife += DT;

		GetOwner()->Animator2D()->SetCurAnim(nullptr);
		GetOwner()->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_1, CAssetMgr::GetInst()->Load<CTexture>(L"BloodMark", L"texture\\BloodMark.png"));
		GetOwner()->GetRenderComponent()->GetDynamicMaterial();

		const float width = CAssetMgr::GetInst()->FindAsset<CTexture>(L"BloodMark")->GetWidth();
		const float height = CAssetMgr::GetInst()->FindAsset<CTexture>(L"BloodMark")->GetHeight();
		Vec2 vSize = Vec2(32.f /( float)width, 32.f / (float)height);
		Vec2 vLT = Vec2(vSize.x * m_fidx, 0.f);
		Vec2 vBg = Vec2(200.f/(float)width, 200.f /(float)height);

		GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_0, vLT); //UV
		GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_1, vBg); //Bg
		GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_2, vSize); //Size
		GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, m_fAlpha); //알파

		if (m_fLife > 3.f)
		{
			m_fAlpha -= 0.9f * DT;
			GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, m_fAlpha); //알파
		}

		if (0.f >= m_fAlpha)
		{
			Dead();
		}
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

