#include "pch.h"
#include "CPlayerAttackScript.h"

CPlayerAttackScript::CPlayerAttackScript()
	: CScript(PLAYERATTACKSCRIPT)
	, m_AttCol(nullptr)
	, m_preAttCol(nullptr)
	, m_bAttacking(false)
	, m_Dir(eDIR::NONE)
{
}

CPlayerAttackScript::~CPlayerAttackScript()
{
}

void CPlayerAttackScript::begin()
{
	// ÇÁ¸®Æé
	// Att Collider
	//m_preAttCol = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Player_AttCollider.pref", L"prefab\\Player_AttCollider.pref");
	//m_AttCol = m_preAttCol->GetProtoGameObj();
	//m_AttCol->MeshRender()->GetDynamicMaterial();
}

void CPlayerAttackScript::tick()
{

}

void CPlayerAttackScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CPlayerAttackScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CPlayerAttackScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CPlayerAttackScript::SaveToFile(FILE* _File)
{
}

void CPlayerAttackScript::LoadFromFile(FILE* _File)
{
}
