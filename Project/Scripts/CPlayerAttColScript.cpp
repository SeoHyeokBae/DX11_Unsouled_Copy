#include "pch.h"
#include "CPlayerAttColScript.h"

CPlayerAttColScript::CPlayerAttColScript()
	: CScript (PLAYERATTCOLSCRIPT)
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
	m_vOriginScale = m_Collider->GetOffsetScale();
}

void CPlayerAttColScript::tick()
{
	m_fDuration += DT;
	m_sCurState = m_Owner->StateMachine()->GetFSM()->GetCurStateName();
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
	}

	if (m_sCurState == L"DashAttState") 
		m_Collider->SetOffsetScale(Vec2(40.f,15.f));

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
	eDIR dir = m_Owner->GetDir();
	if (m_sCurState == L"AttackState")
	{
		int combo = *((int*)m_Owner->StateMachine()->GetBlackboardData(L"Combo"));
			
		if (_OtherObj->GetName() == L"Zombie")
		{
			Vec2 vVelocity = _OtherObj->Movement()->GetVelocity();
			switch (dir)
			{
			case eDIR::UP:
				if(combo == 3)
					vVelocity += Vec2(0.f, 30.f);
				else 
					vVelocity = Vec2(0.f, 10.f);
				break;
			case eDIR::DOWN:
				if (combo == 3)
					vVelocity += Vec2(0.f, -30.f);
				else
					vVelocity = (Vec2(0.f, -10.f));
				break;
			case eDIR::LEFT:
				if (combo == 3)
					vVelocity += (Vec2(-30.f, 0.f));
				else
					vVelocity = (Vec2(-10.f, 0.f));
				break;
			case eDIR::RIGHT:
				if (combo == 3)
					vVelocity += (Vec2(30.f, 0.f));
				else
					vVelocity = (Vec2(10.f, 0.f));
				break;
			case eDIR::UPLEFT:
				if (combo == 3)
					vVelocity += (Vec2(-30.f, 30.f));
				else
					vVelocity = (Vec2(-10.f, 10.f));
				break;
			case eDIR::UPRIGHT:
				if (combo == 3)
					vVelocity = (Vec2(30.f, 30.f));
				else
					vVelocity = (Vec2(10.f, 10.f));
				break;
			case eDIR::DOWNLEFT:
				if (combo == 3)
					vVelocity = (Vec2(-30.f, -30.f));
				else
					vVelocity += (Vec2(-10.f, -10.f));
				break;
			case eDIR::DOWNRIGHT:
				if (combo == 3)
					vVelocity += (Vec2(30.f, -30.f));
				else
					vVelocity = (Vec2(10.f, -10.f));
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
			switch (dir)
			{
			case eDIR::UP:
				vVelocity += Vec2(0.f, 45.f);
				break;
			case eDIR::DOWN:
				vVelocity += Vec2(0.f, -45.f);
				break;
			case eDIR::LEFT:
				vVelocity += (Vec2(-45.f, 0.f));
				break;
			case eDIR::RIGHT:
				vVelocity += (Vec2(45.f, 0.f));
				break;
			case eDIR::UPLEFT:
				vVelocity += (Vec2(-45.f, 45.f));
				break;
			case eDIR::UPRIGHT:
				vVelocity = (Vec2(45.f, 45.f));
				break;
			case eDIR::DOWNLEFT:
				vVelocity = (Vec2(-45.f, -45.f));
				break;
			case eDIR::DOWNRIGHT:
				vVelocity += (Vec2(45.f, -45.f));
				break;
			}
			_OtherObj->Movement()->SetVelocity(vVelocity);
		}
	}

}

void CPlayerAttColScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
	m_Collider->SetOffsetScale(m_vOriginScale);
}




void CPlayerAttColScript::SaveToFile(FILE* _File)
{
}

void CPlayerAttColScript::LoadFromFile(FILE* _File)
{
}

