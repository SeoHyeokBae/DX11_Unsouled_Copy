#include "pch.h"
#include "CPlayerAttColScript.h"

CPlayerAttColScript::CPlayerAttColScript()
	: CScript (PLAYERATTCOL)
	, m_Owner(nullptr)
	, m_Collider(nullptr)
	, m_fDuration(0.f)
{
}

CPlayerAttColScript::~CPlayerAttColScript()
{
}
void CPlayerAttColScript::begin()
{
	m_Owner = GetOwner()->GetParent();
	m_Collider = GetOwner()->Collider2D();
}

void CPlayerAttColScript::tick()
{
	m_fDuration += DT;
	eDIR dir = m_Owner->GetDir();
	switch (dir)
	{
	case eDIR::UP:
		m_Collider->SetOffsetPos(Vec2(0.f, 18.f));
		break;
	case eDIR::DOWN:
		m_Collider->SetOffsetPos(Vec2(0.f, -18.f));
		break;
	case eDIR::LEFT:
		m_Collider->SetOffsetPos(Vec2(-18.f, 0.f));
		break;
	case eDIR::RIGHT:
		m_Collider->SetOffsetPos(Vec2(18.f, 0.f));
		break;
	case eDIR::UPLEFT:
		m_Collider->SetOffsetPos(Vec2(-15.f, 15.f));
		break;
	case eDIR::UPRIGHT:
		m_Collider->SetOffsetPos(Vec2(15.f, 15.f));
		break;
	case eDIR::DOWNLEFT:
		m_Collider->SetOffsetPos(Vec2(-15.f, -15.f));
		break;
	case eDIR::DOWNRIGHT:
		m_Collider->SetOffsetPos(Vec2(15.f, -15.f));
		break;
	case eDIR::NONE:
		break;
	default:
		break;
	}

	m_sCurState = m_Owner->StateMachine()->GetFSM()->GetCurStateName();
	if (m_sCurState == L"AttackState" || m_sCurState == L"DashAttState")
	{
		ColliderOn();
	}
	else if (m_fDuration > 0.5f)
	{
		m_fDuration = 0.f;
		ColliderOff();
	}
}


void CPlayerAttColScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CPlayerAttColScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
	m_sCurState = m_Owner->StateMachine()->GetFSM()->GetCurStateName();

	if (m_sCurState == L"AttackState")
	{
		 
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

