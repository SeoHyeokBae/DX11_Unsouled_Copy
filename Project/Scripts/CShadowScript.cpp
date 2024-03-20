#include "pch.h"
#include "CShadowScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CGameObject.h>
#include <Engine/CAnim.h>

CShadowScript::CShadowScript()
	: CScript(SHADOWSCRIPT)
	, m_Obj(nullptr)
	, m_Anim(nullptr)
	, m_MeshRender(nullptr)

{

}

CShadowScript::~CShadowScript()
{
}

void CShadowScript::begin()
{
	m_Obj = new CGameObject;
	m_Obj->SetName(L"shadow");

	m_Obj->AddComponent(new CTransform);


	m_Anim = GetOwner()->Animator2D();
	m_MeshRender = GetOwner()->MeshRender();

	m_Obj->AddComponent(m_Anim);
	m_Obj->AddComponent(m_MeshRender);

	CAnimator2D* anim = m_Obj->Animator2D();
	m_Obj->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos());
	m_Obj->Transform()->SetRelativeScale(GetOwner()->Transform()->GetRelativeScale());

	Vec3 vRot = m_Obj->Transform()->GetRelativeRotation();
	vRot.y += XM_PI * -0.25f;
	vRot.x += XM_PI * 0.3f;
	m_Obj->Transform()->SetRelativeRotation(vRot);

	CLevel* level = CLevelMgr::GetInst()->GetCurrentLevel();
	level->AddObject(m_Obj, L"default", false);
}

void CShadowScript::tick()
{

	m_Obj->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos());
	m_Obj->Animator2D()->SetCurAnim(m_Obj->Animator2D()->GetCurAnim());
}

void CShadowScript::SaveToFile(FILE* _File)
{
}

void CShadowScript::LoadFromFile(FILE* _File)
{
}

