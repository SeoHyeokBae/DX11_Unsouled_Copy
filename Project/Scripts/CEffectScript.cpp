#include "pch.h"
#include "CEffectScript.h"

CEffectScript::CEffectScript()
	: CScript(EFFECTSCRIPT)
	, m_EffectObj(nullptr)
	, m_ScrMgr(nullptr)
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
	m_EffectObj->AddComponent(new CAnimator2D);
	m_EffectObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	//CAssetMgr::GetInst()->Load<CMaterial>(L"Effect", L"material\\Effect.mtrl");
	//m_EffectObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Effect"));
	m_EffectObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"effectmtrl"));
	m_EffectObj->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	m_EffectObj->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));
	m_EffectObj->Transform()->SetRelativePos(Vec3(0.f, -10.f, -10.f));

	m_EffectObj->Animator2D()->AddAnim(L"HitCircle", L"anim\\HitCircle.anim");
	m_EffectObj->Animator2D()->AddAnim(L"SwordSpark", L"anim\\SwordSpark.anim");
	m_EffectObj->Animator2D()->AddAnim(L"ChainEffect", L"anim\\ChainEffect.anim");

	GetOwner()->AddChild(m_EffectObj);
	GamePlayStatic::SpawnGameObject(m_EffectObj, 0);

	m_ScrMgr = new CScriptMgr;
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

