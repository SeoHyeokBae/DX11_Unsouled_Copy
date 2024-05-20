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
	, m_vCalculatedPos(Vec2(0.f,0.f))
{
}

CEffectScript::~CEffectScript()
{
}


void CEffectScript::begin()
{
	// ������ų ����Ʈ ������Ʈ ������
	// ����Ʈ �߻��� ������Ʈ ������ ����
	m_EffectPrefab = CAssetMgr::GetInst()->Load<CPrefab>(L"NormalEffect", L"prefab\\NormalEffect.pref");

	assert(m_EffectPrefab->GetProtoGameObj()->Animator2D()); // Animator ������Ʈ�� ����

	// ���Ϳ�����Ʈ�� ���� ���»�� �̹� anim�� ���� �� ����
	if (0 == m_EffectPrefab->GetProtoGameObj()->Animator2D()->GetAnimCount())
	{
		m_EffectPrefab->GetProtoGameObj()->Animator2D()->AddAnim(L"HitCircle", L"anim\\HitCircle.anim");
		m_EffectPrefab->GetProtoGameObj()->Animator2D()->AddAnim(L"SwordSpark", L"anim\\SwordSpark.anim");
		m_EffectPrefab->GetProtoGameObj()->Animator2D()->AddAnim(L"ChainEffect", L"anim\\ChainEffect.anim");
	}

}

void CEffectScript::tick()
{
	// �ִϸ��̼� ���� üũ
	for (UINT i = 1; i < (UINT)eEffectStatus::NONE * 2; i *= 2)
	{
		if (!(m_iStatus & (1 << i)))
			continue;

		map<int, CGameObject*>::iterator iter = m_RegisterObj.find(1 << i);
		CGameObject* effObj = iter->second;
		
		if (effObj->Animator2D()->GetCurAnim()->IsFinish())
		{
			Dead(effObj, i);
		}

	}
}

void CEffectScript::OnEffect(eEffectStatus _status)
{
	// �̹� ����Ʈ Ȱ��ȭ �Ǿ� �ִ°��
	if (m_iStatus & (1 << (UINT)_status))
	{
		return;
		// ���� ����Ʈ�� delete �ϰ� ���ο� ������Ʈ�� ��ü
		map<int, CGameObject*>::iterator iter = m_RegisterObj.find(1 << (UINT)_status);
		CGameObject* effObj = iter->second;
		Dead(effObj, (UINT)_status);
	}

	CGameObject* pNewEffectObj = m_EffectPrefab->Instantiate();
	GamePlayStatic::SpawnGameObject(pNewEffectObj, 0);

	switch (_status)
	{
	case eEffectStatus::CHAIN_EFFECT:
		m_RegisterObj.insert(make_pair(CHAIN, pNewEffectObj));
		m_iStatus |= CHAIN;
		pNewEffectObj->SetName(L"CHAIN_EFF");
		pNewEffectObj->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos());
		pNewEffectObj->Animator2D()->Play(L"ChainEffect",false);
		break;

	case eEffectStatus::SWORD_SPARK:
		m_RegisterObj.insert(make_pair(SPARK, pNewEffectObj));
		m_iStatus |= SPARK;
		pNewEffectObj->SetName(L"SPARK_EFF");
		pNewEffectObj->Transform()->SetRelativePos(Vec3(m_vCalculatedPos.x, m_vCalculatedPos.y,0.f));
		pNewEffectObj->Animator2D()->Play(L"SwordSpark", false);
		OnEffect(eEffectStatus::HIT_CIRCLE);
		break;

	case eEffectStatus::HIT_CIRCLE:
		m_RegisterObj.insert(make_pair(HITCIRCLE, pNewEffectObj));
		m_iStatus |= HITCIRCLE;
		pNewEffectObj->SetName(L"HITCIRCLE_EFF");
		pNewEffectObj->Transform()->SetRelativePos(Vec3(m_vCalculatedPos.x, m_vCalculatedPos.y, 0.f));
		pNewEffectObj->Animator2D()->Play(L"HitCircle", false);
		break;
	}
}

void CEffectScript::Dead(CGameObject* _obj, int _effNum)
{
	_obj->Destroy();
	m_iStatus &= ~(1 << _effNum);
	m_RegisterObj.erase(1 << _effNum);
}

void CEffectScript::SaveToFile(FILE* _File)
{
}

void CEffectScript::LoadFromFile(FILE* _File)
{
}

