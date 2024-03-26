#include "pch.h"
#include "CShadowScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CGameObject.h>

// TODO 
// 발끝 지점 찾기
// 자유롭게 각도 조절 가능하게

CShadowScript::CShadowScript()
	: CScript(SHADOWSCRIPT)
	, m_Obj(nullptr)

{
}

CShadowScript::~CShadowScript()
{
}

void CShadowScript::begin()
{
	m_Obj = new CGameObject;
	m_Obj->SetName(L"Shadow");
	m_Obj->AddComponent(new CTransform);
	m_Obj->AddComponent(new CMeshRender);
	m_Obj->AddComponent(new CAnimator2D);

	m_Obj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	//m_Obj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
	CAssetMgr::GetInst()->Load<CMaterial>(L"TestMtrl", L"material\\testmtrl.mtrl");
	m_Obj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"TestMtrl"));
	m_Obj->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 10);
	
	m_Obj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));
	m_Obj->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos());

	Ptr<CTexture> pAltasTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\link.png", L"texture\\link.png");
	m_Obj->Animator2D()->Create(L"IDLE_UP", pAltasTex, Vec2(0.f, 260.f), Vec2(120.f, 130.f), Vec2(0.f, 0.f), Vec2(200.f, 200.f), 1, 10);
	m_Obj->Animator2D()->Create(L"IDLE_DOWN", pAltasTex, Vec2(0.f, 0.f), Vec2(120.f, 130.f), Vec2(0.f, 0.f), Vec2(200.f, 200.f), 3, 10);
	m_Obj->Animator2D()->Create(L"IDLE_LEFT", pAltasTex, Vec2(0.f, 130.f), Vec2(120.f, 130.f), Vec2(0.f, 0.f), Vec2(200.f, 200.f), 3, 10);
	m_Obj->Animator2D()->Create(L"IDLE_RIGHT", pAltasTex, Vec2(0.f, 390.f), Vec2(120.f, 130.f), Vec2(0.f, 0.f), Vec2(200.f, 200.f), 3, 10);
	m_Obj->Animator2D()->Play(L"IDLE_DOWN");


	Vec3 vRot = m_Obj->Transform()->GetRelativeRotation();
	vRot.y += XM_PI * -0.25f;
	vRot.x += XM_PI * 0.3f;
	vRot.z += XM_PI * 0.05f;

	m_Obj->Transform()->SetRelativeRotation(vRot);

	GamePlayStatic::SpawnGameObject(m_Obj, 5);
	GetOwner()->SetShadow(m_Obj);
}

void CShadowScript::tick()
{
	m_Obj->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos());
	m_Obj->Transform()->SetRelativePos(Vec3(GetOwner()->Transform()->GetRelativePos().x -12.5f
																	, GetOwner()->Transform()->GetRelativePos().y - 12.5f
																	, GetOwner()->Transform()->GetRelativePos().z + 50.f));

}

void CShadowScript::SaveToFile(FILE* _File)
{
}

void CShadowScript::LoadFromFile(FILE* _File)
{
}

