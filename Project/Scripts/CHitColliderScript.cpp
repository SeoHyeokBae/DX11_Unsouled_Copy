#include "pch.h"
#include "CHitColliderScript.h"

#include <Engine/CTaskMgr.h>
#include <Scripts/CBossNiugScript.h>
#include <Scripts/CPlayerScript.h>

CHitColliderScript::CHitColliderScript()
	: CScript(HITCOLLIDERSCRIPT)
	, m_Owner(nullptr)
	, m_bOn(false)
	, m_fDuration(1.f)
	, m_fAcc(0.f)
{
}

CHitColliderScript::~CHitColliderScript()
{
}

void CHitColliderScript::begin()
{
	Animator2D()->AddAnim(L"Niug_RageOfIsno_Explosion", L"anim\\Niug_RageOfIsno_Explosion.anim");

	if (m_Owner->GetName() == L"Boss_Niug")
	{
		Animator2D()->Play(L"Niug_RageOfIsno_Explosion", false);
	}
}

void CHitColliderScript::tick()
{
	m_fAcc += DT;
	if (m_fAcc > m_fDuration)
	{
		Dead();
	}
	
}

void CHitColliderScript::Dead()
{
	GamePlayStatic::DestroyGameObject(GetOwner());
}

void CHitColliderScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
	if (m_Owner->GetName() == L"Boss_Niug" && !_OtherObj->GetParent()->GetScript<CPlayerScript>()->IsDamaged())
	{
		_OtherObj->GetParent()->GetScript<CPlayerScript>()->Damaged();
		m_Owner->GetScript<CBossNiugScript>()->Hit();
	}
}

void CHitColliderScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CHitColliderScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CHitColliderScript::SaveToFile(FILE* _File)
{
}

void CHitColliderScript::LoadFromFile(FILE* _File)
{
}

