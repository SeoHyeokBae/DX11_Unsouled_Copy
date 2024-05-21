#include "pch.h"
#include "CPlayerAttColScript.h"

#include <Scripts/CPlayerScript.h>

CPlayerAttColScript::CPlayerAttColScript()
	: CScript (PLAYERATTCOLSCRIPT)
	, m_pOwner(nullptr)
	, m_Collider(nullptr)
	, m_fDuration(0.f)
	, m_Dir(eDIR::NONE)
	, m_vPrvPos(Vec2(0.f,0.f))
{
}

CPlayerAttColScript::~CPlayerAttColScript()
{
}

void CPlayerAttColScript::begin()
{
	m_pOwner = GetOwner()->GetParent();
	m_Collider = GetOwner()->Collider2D();
	m_vPrvScale = m_Collider->GetOffsetScale();
	m_vPrvPos = m_Collider->GetOffsetPos();
}

void CPlayerAttColScript::tick()
{
	m_fDuration += DT;
	m_sCurState = m_pOwner->StateMachine()->GetFSM()->GetCurStateName();

	Vec3 vDir = Vec3(1.f, 0.f, 0.f);
	m_Dir = m_pOwner->GetDir();
	switch (m_Dir)
	{
	case eDIR::UP:
		vDir = Vec3(0.f, 1.f, 0.f);
		break;
	case eDIR::DOWN:
		vDir = Vec3(0.f, -1.f, 0.f);
		break;
	case eDIR::LEFT:
		vDir = Vec3(-1.f, 0.f, 0.f);
		break;
	case eDIR::RIGHT:
		vDir = Vec3(1.f, 0.f, 0.f);
		break;
	case eDIR::UPLEFT:
		vDir = Vec3(-1.f, 1.f, 0.f);
		break;
	case eDIR::UPRIGHT:
		vDir = Vec3(1.f, 1.f, 0.f);
		break;
	case eDIR::DOWNLEFT:
		vDir = Vec3(-1.f, -1.f, 0.f);
		break;
	case eDIR::DOWNRIGHT:
		vDir = Vec3(1.f, -1.f, 0.f);
		break;
	}

	vDir.Normalize();
	float angle = acos(vDir.Dot(Vec3(1.0f, 0.f, 0.f)) / vDir.Length());

	if (0.f < vDir.y)
	{
		Transform()->SetRelativeRotation(Vec3(0, 0, angle));
	}
	else
	{
		Transform()->SetRelativeRotation(Vec3(0, 0, -angle));
	}
}

void CPlayerAttColScript::SetScale(int _width, int _height)
{
	// col 절반 크기 이동 > topbody 사이즈만큼 뺌, y pos 는 0고정
	// 기존 col 정보 저장
	m_vPrvScale = m_Collider->GetOffsetScale();
	m_vPrvPos = m_Collider->GetOffsetPos();
	Vec2 vHalfSize = GetOwner()->GetParent()->GetScript<CPlayerScript>()->GetBodyCol()->Collider2D()->GetOffsetScale() / 2.f;
	
	m_Collider->SetOffsetScale(Vec2(_width, _height));
	m_Collider->SetOffsetPos(Vec2(_width / 2.f - vHalfSize.x, 0.f));
}

void CPlayerAttColScript::ReturnScale()
{
	m_Collider->SetOffsetScale(m_vPrvScale);
	m_Collider->SetOffsetPos(m_vPrvPos);
}

void CPlayerAttColScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CPlayerAttColScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
	if (!GetOwner()->GetParent()->GetScript<CPlayerScript>()->IsCanHit()) return;
	GetOwner()->GetParent()->GetScript<CPlayerScript>()->OffCanHit();

	m_sCurState = m_pOwner->StateMachine()->GetFSM()->GetCurStateName();
	eDIR dir = m_pOwner->GetDir();


	if (m_sCurState == L"AttackState")
	{
		int combo = *((int*)m_pOwner->StateMachine()->GetBlackboardData(L"Combo"));
			
		if (_OtherObj->GetName() == L"Zombie")
		{
			Vec2 vVelocity = _OtherObj->Movement()->GetVelocity();
			switch (dir)
			{
			case eDIR::UP:
				if(combo == 3)
					vVelocity = Vec2(0.f, 500.f);
				else 
					vVelocity = Vec2(0.f, 300.f);
				break;
			case eDIR::DOWN:
				if (combo == 3)
					vVelocity = Vec2(0.f, -500.f);
				else
					vVelocity = (Vec2(0.f, -300.f));
				break;
			case eDIR::LEFT:
				if (combo == 3)
					vVelocity = (Vec2(-500.f, 0.f));
				else
					vVelocity = (Vec2(-300.f, 0.f));
				break;
			case eDIR::RIGHT:
				if (combo == 3)
					vVelocity = (Vec2(500.f, 0.f));
				else
					vVelocity = (Vec2(300.f, 0.f));
				break;
			case eDIR::UPLEFT:
				if (combo == 3)
					vVelocity = (Vec2(-500.f, 500.f));
				else
					vVelocity = (Vec2(-300.f, 300.f));
				break;
			case eDIR::UPRIGHT:
				if (combo == 3)
					vVelocity = (Vec2(500.f, 500.f));
				else
					vVelocity = (Vec2(250.f, 250.f));
				break;
			case eDIR::DOWNLEFT:
				if (combo == 3)
					vVelocity = (Vec2(-500.f, -500.f));
				else
					vVelocity = (Vec2(-250.f , -250.f));
				break;
			case eDIR::DOWNRIGHT:
				if (combo == 3)
					vVelocity = (Vec2(500.f, -500.f));
				else
					vVelocity = (Vec2(250.f, -250.f));
				break;
			}
			_OtherObj->Movement()->SetVelocity(vVelocity);
		}
	}

	else if (m_sCurState == L"DashAttState")
	{
		if (_OtherObj->GetName() == L"Zombie")
		{
			Vec2 vVelocity = _OtherObj->Movement()->GetVelocity();
			vVelocity += GetOwner()->GetParent()->Movement()->GetVelocity();
			switch (dir)
			{
			case eDIR::UP:
				vVelocity += Vec2(0.f, 250.f);
				break;
			case eDIR::DOWN:
				vVelocity += Vec2(0.f, -250.f);
				break;
			case eDIR::LEFT:
				vVelocity += (Vec2(-250.f, 0.f));
				break;
			case eDIR::RIGHT:
				vVelocity += (Vec2(250.f, 0.f));
				break;
			case eDIR::UPLEFT:
				vVelocity += (Vec2(-250.f, 250.f));
				break;
			case eDIR::UPRIGHT:
				vVelocity += (Vec2(250.f, 250.f));
				break;
			case eDIR::DOWNLEFT:
				vVelocity += (Vec2(-250.f, -250.f));
				break;
			case eDIR::DOWNRIGHT:
				vVelocity += (Vec2(250.f, -250.f));
				break;
			}
			_OtherObj->Movement()->SetVelocity(vVelocity);
		}
	}

}

void CPlayerAttColScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}




void CPlayerAttColScript::SaveToFile(FILE* _File)
{
}

void CPlayerAttColScript::LoadFromFile(FILE* _File)
{
}

