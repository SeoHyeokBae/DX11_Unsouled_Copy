#include "pch.h"
#include "CEffectScript.h"

#include "Engine/CAssetMgr.h"
#include "Engine/CLevelMgr.h"
#include "Engine/CLevel.h"



CEffectScript::CEffectScript()
	: CScript(EFFECTSCRIPT)
	, m_EffectPrefab(nullptr)
	, m_ScrMgr(nullptr)
	, m_iStatus(0)
{
}

CEffectScript::~CEffectScript()
{
}


void CEffectScript::begin()
{
	// 생성시킬 이펙트 오브젝트 프리팹
	// 이펙트 발생시 오브젝트 복제본 생성
	m_EffectPrefab = CAssetMgr::GetInst()->Load<CPrefab>(L"NormalEffect", L"prefab\\NormalEffect.pref");

	assert(m_EffectPrefab->GetProtoGameObj()->Animator2D()); // Animator 컴포넌트가 없음

	m_EffectPrefab->GetProtoGameObj()->Animator2D()->AddAnim(L"HitCircle", L"anim\\HitCircle.anim");
	m_EffectPrefab->GetProtoGameObj()->Animator2D()->AddAnim(L"SwordSpark", L"anim\\SwordSpark.anim");
	m_EffectPrefab->GetProtoGameObj()->Animator2D()->AddAnim(L"ChainEffect", L"anim\\ChainEffect.anim");
}

void CEffectScript::tick()
{
	
	// 애니메이션 종료 체크
	for (UINT i = 0; i < STATUS_MAX; i++)
	{
		if (!(m_iStatus & (1 << i)))
			continue;

		map<int, CGameObject*>::iterator iter = m_RegisterObj.find(1 << i);
		 
		CGameObject* effObj = iter->second;
		if (effObj->Animator2D()->GetCurAnim()->IsFinish())
		{
			Dead(effObj);
			
			m_iStatus &= ~(1 << i);
			m_RegisterObj.erase(1 << i);
		}
	}


}

void CEffectScript::OnEffect(eEffectStatus _status)
{
	CGameObject* pNewEffectObj = m_EffectPrefab->Instantiate();
	
	GetOwner()->AddChild(pNewEffectObj);
	GamePlayStatic::SpawnGameObject(pNewEffectObj, 0);

	switch (_status)
	{
	case eEffectStatus::CHAIN_EFFECT:
		m_RegisterObj.insert(make_pair(CHAIN, pNewEffectObj));
		m_iStatus |= CHAIN;
		pNewEffectObj->SetName(L"CHAIN_EFF");
		pNewEffectObj->Animator2D()->Play(L"ChainEffect",false);
		break;

	case eEffectStatus::SWORD_SPARK:
		m_RegisterObj.insert(make_pair(SPARK, pNewEffectObj));
		m_iStatus |= SPARK;
		pNewEffectObj->SetName(L"SPARK_EFF");
		pNewEffectObj->Animator2D()->Play(L"SwordSpark", false);
		break;
	}
}

void CEffectScript::Dead(CGameObject* _obj)
{
	int LayerIdx = _obj->DisconnectWithParent();
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	pCurLevel->AddObject(_obj, LayerIdx, false);
	_obj->Destroy();
}


void CEffectScript::SaveToFile(FILE* _File)
{
}

void CEffectScript::LoadFromFile(FILE* _File)
{
}

