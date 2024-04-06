#include "pch.h"
#include "CMonsterScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>


#include "../Client/CIdleState.h"
#include "../Client/CTraceState.h"

CMonsterScript::CMonsterScript()
	: CScript(MONSTERSCRIPT)
	, m_DetectRange(200.f)
	, m_Speed(100.f)
{

}

CMonsterScript::~CMonsterScript()
{
}

void CMonsterScript::begin()
{
	// 동적재질로 변경
	GetRenderComponent()->GetDynamicMaterial();

	if (GetOwner()->GetName() == L"Zomibie" || GetOwner()->GetName() == L"Room_Zomibie")
	{
		m_Speed = 50.f;
		m_DetectRange = 200.f;
	}
	else if (GetOwner()->GetName() == L"Boss_Niug")
	{
		m_Speed = 150.f;
		m_DetectRange = 400.f;
	}

	StateMachine()->AddBlackboardData(L"DetectRange", BB_DATA::FLOAT, &m_DetectRange);
	StateMachine()->AddBlackboardData(L"Speed", BB_DATA::FLOAT, &m_Speed);

	// 플레이어를 블랙보드에 등록
	CGameObject* pPlayer = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Player");
	if (pPlayer)
	{
		StateMachine()->AddBlackboardData(L"TargetObject", BB_DATA::OBJECT, pPlayer);
	}
}

void CMonsterScript::tick()
{
}

void CMonsterScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{

}

void CMonsterScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{	
	//Vec2 vVelocity = _OtherObj->GetParent()->Movement()->GetVelocity();
	//_OtherObj->GetParent()->Movement()->SetVelocity(Vec2(vVelocity.x -vVelocity.x * 0.8, 0.f));
}

void CMonsterScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CMonsterScript::SaveToFile(FILE* _File)
{
}

void CMonsterScript::LoadFromFile(FILE* _File)
{
}

void CMonsterScript::init()
{
}
