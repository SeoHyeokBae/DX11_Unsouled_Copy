#include "pch.h"
#include "CEffectScript.h"

CEffectScript::CEffectScript()
	: CScript(EFFECTSCRIPT)
	, m_EffectObj(nullptr)
{
}

CEffectScript::~CEffectScript()
{
}

void CEffectScript::begin()
{
	m_EffectObj = new CGameObject;
	m_EffectObj->SetName(L"Effect");
	m_EffectObj->AddComponent(new CTransform);
	m_EffectObj->AddComponent(new CMeshRender);
	m_EffectObj->SelfShadow(true);

	m_EffectObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	CAssetMgr::GetInst()->Load<CMaterial>(L"Effect", L"material\\Effect.mtrl");
	m_EffectObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Effect"));
	m_EffectObj->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 10);

	m_EffectObj->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));
}

void CEffectScript::tick()
{
}

void CEffectScript::SaveToFile(FILE* _File)
{
}

void CEffectScript::LoadFromFile(FILE* _File)
{
}

