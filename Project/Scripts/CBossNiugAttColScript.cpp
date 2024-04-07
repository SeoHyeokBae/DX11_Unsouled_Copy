#include "pch.h"
#include "CBossNiugAttColScript.h"

#include "CBossNiugScript.h"

// Basic Att Lock ON

CBossNiugAttColScript::CBossNiugAttColScript()
	:CScript(BOSSNIUGATTCOLSCRIPT)
	, m_bOn(false)
{
}

CBossNiugAttColScript::~CBossNiugAttColScript()
{
}

void CBossNiugAttColScript::begin()
{
}

void CBossNiugAttColScript::tick()
{
	CGameObject* pSelf = GetOwner()->GetParent();
	CGameObject* pTarget = (CGameObject*)GetOwner()->GetParent()->StateMachine()->GetBlackboardData(L"TargetObject");
	Vec3 vDir = pTarget->Transform()->GetWorldPos() - Transform()->GetWorldPos();
	vDir.Normalize();
	
	// 플레이어를 향해 방향 조절
	if (pSelf->StateMachine()->GetFSM()->GetCurStateName() == L"BasicAttState" && !pSelf->Animator2D()->GetCurAnim()->IsFinish())
		return;

	float angle = acos(vDir.Dot(Vec3(1.0f, 0.f, 0.f)) / vDir.Length());
	if (0.f < vDir.y)
	{
		Transform()->SetRelativeRotation(Vec3(0,0, angle));
	}
	else
	{
		Transform()->SetRelativeRotation(Vec3(0, 0, -angle));
	}
}

void CBossNiugAttColScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CBossNiugAttColScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
	CGameObject* pOwner = GetOwner()->GetParent();
	if (m_bOn && _OtherObj->GetName() == L"Player_HitBox")
	{
		m_bOn = false;
	}
}

void CBossNiugAttColScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CBossNiugAttColScript::SaveToFile(FILE* _File)
{
}

void CBossNiugAttColScript::LoadFromFile(FILE* _File)
{
}


