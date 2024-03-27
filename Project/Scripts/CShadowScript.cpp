#include "pch.h"
#include "CShadowScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CGameObject.h>

// TODO 
// �߳� ���� ã��
// �����Ӱ� ���� ���� �����ϰ�

// ���� ������Ʈ, �����ν��̴� = Mtrl��  Effect Shader (Graphic Shader)����
// But Owner�� �ִϸ��̼��� �����Ҷ� �ִϸ��̼� StdShader �� �׷����Ƿ� 
// StdShader �ڵ忡�� ���� ����

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
	m_Obj->SelfShadow(true);
	m_Obj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	//m_Obj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
	CAssetMgr::GetInst()->Load<CMaterial>(L"Shadow", L"material\\Shadow.mtrl");
	m_Obj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Shadow"));
	m_Obj->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 10);
	
	m_Obj->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));
	m_Obj->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos());


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
	m_Obj->Transform()->SetRelativePos(Vec3(GetOwner()->Transform()->GetRelativePos().x
																	, GetOwner()->Transform()->GetRelativePos().y
																	, GetOwner()->Transform()->GetRelativePos().z + 50.f));

}

void CShadowScript::SaveToFile(FILE* _File)
{
}

void CShadowScript::LoadFromFile(FILE* _File)
{
}

