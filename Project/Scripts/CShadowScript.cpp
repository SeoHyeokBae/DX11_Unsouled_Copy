#include "pch.h"
#include "CShadowScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CGameObject.h>

// TODO 
// 자유롭게 각도 조절 가능하게

// 블렌드 스테이트, 도메인쉐이더 = Mtrl이  Effect Shader (Graphic Shader)보유
// 몬스터와 플레이어가 같은 mtrl 을 참조하는데 어떻게 다르게 그려지는가 ?

CShadowScript::CShadowScript()
	: CScript(SHADOWSCRIPT)
	, m_ShadowObj(nullptr)

{ 
}

CShadowScript::~CShadowScript()
{
}

void CShadowScript::begin()
{
	m_ShadowObj = new CGameObject;
	m_ShadowObj->SetName(L"Shadow");
	m_ShadowObj->AddComponent(new CTransform);
	m_ShadowObj->AddComponent(new CMeshRender);
	m_ShadowObj->SelfShadow(true);

	m_ShadowObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	CAssetMgr::GetInst()->Load<CMaterial>(L"Shadow", L"material\\Shadow.mtrl");
	m_ShadowObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Shadow"));
	m_ShadowObj->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 10);
	// 동적재질로 변경
	m_ShadowObj->MeshRender()->GetRenderComponent()->GetDynamicMaterial();
	
	m_ShadowObj->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));
	m_ShadowObj->Transform()->SetRelativePos(Vec3(0.f,0.f,50.f));


	Vec3 vRot = m_ShadowObj->Transform()->GetRelativeRotation();
	vRot.y += XM_PI * -0.25f;
	vRot.x += XM_PI * 0.3f;
	vRot.z += XM_PI * 0.05f;

	m_ShadowObj->Transform()->SetRelativeRotation(vRot);
	GetOwner()->AddChild(m_ShadowObj);

	GamePlayStatic::SpawnGameObject(m_ShadowObj, 5);
}

void CShadowScript::tick()
{
	// Owner에서 애니메이션 보유x
	//Ptr<CTexture> tex = m_ShadowObj->MeshRender()->GetMaterial()->GetTexParam(TEX_PARAM::TEX_0);
	//assert(m_ShadowObj->Animator2D() || nullptr != m_ShadowObj->MeshRender()->GetMaterial()->GetTexParam(TEX_PARAM::TEX_0));
	int a = 0;
}

void CShadowScript::SaveToFile(FILE* _File)
{
}

void CShadowScript::LoadFromFile(FILE* _File)
{
}

