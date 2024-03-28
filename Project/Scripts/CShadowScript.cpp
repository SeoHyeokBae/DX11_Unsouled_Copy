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
// �θ�Script���� Animator ��� �����

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
	// Owner���� �ִϸ��̼� ����x
	assert(m_ShadowObj->Animator2D());

	//m_Obj->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos());
	//m_Obj->Transform()->SetRelativePos(Vec3(GetOwner()->Transform()->GetRelativePos().x
	//																, GetOwner()->Transform()->GetRelativePos().y
	//																, GetOwner()->Transform()->GetRelativePos().z + 50.f));

}

void CShadowScript::SaveToFile(FILE* _File)
{
}

void CShadowScript::LoadFromFile(FILE* _File)
{
}

