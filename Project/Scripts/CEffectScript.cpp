#include "pch.h"
#include "CEffectScript.h"

#include "Engine/CAssetMgr.h"
#include "Engine/CLevelMgr.h"
#include "Engine/CLevel.h"

#include "CBloodScript.h"

#define RED_TIME 0.1f



CEffectScript::CEffectScript()
	: CScript(EFFECTSCRIPT)
	, m_EffectPrefab(nullptr)
	, m_BloodPrefab(nullptr)
	, m_ScrMgr(nullptr)
	, m_iStatus(0)
	, m_vCalculatedPos(Vec2(0.f,0.f))
	, m_vCalculatedRot(Vec3(1.f,1.f,1.f))
	, m_bRed(false)
	, m_bRedTime(0.f)
	, m_eDir(eBloodDir::NONE)
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
	m_BloodPrefab = CAssetMgr::GetInst()->Load<CPrefab>(L"Blood", L"prefab\\Blood.pref");

	assert(m_EffectPrefab->GetProtoGameObj()->Animator2D()); // Animator 컴포넌트가 없음

	// 몬스터오브젝트도 같은 에셋사용 이미 anim이 있을 수 있음
	if (0 == m_EffectPrefab->GetProtoGameObj()->Animator2D()->GetAnimCount())
	{
		m_EffectPrefab->GetProtoGameObj()->Animator2D()->AddAnim(L"HitCircle", L"anim\\HitCircle.anim");
		m_EffectPrefab->GetProtoGameObj()->Animator2D()->AddAnim(L"SwordSpark", L"anim\\SwordSpark.anim");
		m_EffectPrefab->GetProtoGameObj()->Animator2D()->AddAnim(L"ChainEffect", L"anim\\ChainEffect.anim");

		// Blood Effect
		m_EffectPrefab->GetProtoGameObj()->Animator2D()->AddAnim(L"BloodDownLeft", L"anim\\BloodDownLeft.anim");
		m_EffectPrefab->GetProtoGameObj()->Animator2D()->AddAnim(L"BloodLeftDown", L"anim\\BloodLeftDown.anim");
		m_EffectPrefab->GetProtoGameObj()->Animator2D()->AddAnim(L"BloodLeft1", L"anim\\BloodLeft1.anim");
		m_EffectPrefab->GetProtoGameObj()->Animator2D()->AddAnim(L"BloodLeft2", L"anim\\BloodLeft2.anim");
		m_EffectPrefab->GetProtoGameObj()->Animator2D()->AddAnim(L"BloodLeftUp", L"anim\\BloodLeftUp.anim");
		m_EffectPrefab->GetProtoGameObj()->Animator2D()->AddAnim(L"BloodUpLeft", L"anim\\BloodUpLeft.anim");

		m_EffectPrefab->GetProtoGameObj()->Animator2D()->AddAnim(L"BloodRightDown", L"anim\\BloodRightDown.anim");
		m_EffectPrefab->GetProtoGameObj()->Animator2D()->AddAnim(L"BloodDownRight", L"anim\\BloodDownRight.anim");
		m_EffectPrefab->GetProtoGameObj()->Animator2D()->AddAnim(L"BloodRight1", L"anim\\BloodRight.anim");
		m_EffectPrefab->GetProtoGameObj()->Animator2D()->AddAnim(L"BloodRight2", L"anim\\BloodRight2.anim");
		m_EffectPrefab->GetProtoGameObj()->Animator2D()->AddAnim(L"BloodUpRight", L"anim\\BloodUpRight.anim");
		m_EffectPrefab->GetProtoGameObj()->Animator2D()->AddAnim(L"BloodRightUp", L"anim\\BloodRightUp.anim");
	}

	if (0 == m_BloodPrefab->GetProtoGameObj()->Animator2D()->GetAnimCount())
	{
		m_BloodPrefab->GetProtoGameObj()->Animator2D()->AddAnim(L"BloodSmall", L"anim\\BloodSmall.anim");
		m_BloodPrefab->GetProtoGameObj()->Animator2D()->AddAnim(L"BloodNormal", L"anim\\BloodNormal.anim");
	}
}

void CEffectScript::tick()
{
	// 애니메이션 종료 체크
	for (UINT i = 0; i < (UINT)eEffectStatus::NONE; ++i)
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

	// 피격시 붉은색
	if (m_bRed)
	{
		if (0.f == m_bRedTime)
		{
			GetOwner()->GetRenderComponent()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_3, 2);
			CreateBlood();
		}

		m_bRedTime += DT;
		if (m_bRedTime >= RED_TIME)
		{
			m_bRedTime = 0.f;
			m_bRed = false;
			GetOwner()->GetRenderComponent()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_3, 0);
		}
	}
}

void CEffectScript::OnEffect(eEffectStatus _status)
{
	// 이미 이펙트 활성화 되어 있는경우
	if (m_iStatus & (1 << (UINT)_status))
	{
		//return; // 비용이 적으나 발동위치 문제
		// 기존 이펙트를 delete 하고 새로운 오브젝트로 교체 // 이펙트 타이밍 좋으나 비용소모가 클것같음
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

	case eEffectStatus::SWORD_SPARK_EFFECT:
		m_RegisterObj.insert(make_pair(SWORDSPARK, pNewEffectObj));
		m_iStatus |= SWORDSPARK;
		pNewEffectObj->SetName(L"SPARK_EFF");
		pNewEffectObj->Transform()->SetRelativePos(Vec3(m_vCalculatedPos.x, m_vCalculatedPos.y,-1.f));
		pNewEffectObj->Transform()->SetRelativeRotation(m_vCalculatedRot);
		pNewEffectObj->Animator2D()->Play(L"SwordSpark", false);
		OnEffect(eEffectStatus::HITCIRCLE_EFFECT);
		OnEffect(eEffectStatus::BLOOD_EFFECT);
		break;

	case eEffectStatus::HITCIRCLE_EFFECT:
		m_RegisterObj.insert(make_pair(HITCIRCLE, pNewEffectObj));
		m_iStatus |= HITCIRCLE;
		pNewEffectObj->SetName(L"HITCIRCLE_EFF");
		pNewEffectObj->Transform()->SetRelativePos(Vec3(m_vCalculatedPos.x, m_vCalculatedPos.y, -1.f));
		pNewEffectObj->Animator2D()->Play(L"HitCircle", false);
		break;

	case eEffectStatus::BLOOD_EFFECT:
		m_RegisterObj.insert(make_pair(BLOOD, pNewEffectObj));
		m_iStatus |= BLOOD; 
		pNewEffectObj->SetName(L"BLOOD_EFF");
		pNewEffectObj->Transform()->SetRelativePos(Vec3(m_vCalculatedPos.x, m_vCalculatedPos.y, -1.f));

		RandPlayBloodEff(pNewEffectObj);
		//pNewEffectObj->Animator2D()->Play(L"BloodRightUp", false);
		break;
	}
}

void CEffectScript::RandPlayBloodEff(CGameObject* _BloodObj)
{
	int randNum = rand() % 4;
	switch (m_eDir)
	{
	case eBloodDir::RIGHTDOWN:
		break;
	case eBloodDir::RIGHTUP:
		randNum += 2;
		break;
	case eBloodDir::LEFTDOWN:
		randNum += 10;
		break;
	case eBloodDir::LEFTUP:
		randNum += 12;
		break;
	}

	switch (randNum)
	{
	case 0 :
		_BloodObj->Animator2D()->Play(L"BloodLeftDown", false);
		break;
	case 1:
		_BloodObj->Animator2D()->Play(L"BloodDownLeft", false);
		break;
	case 2:
		_BloodObj->Animator2D()->Play(L"BloodLeft1", false);
		break;
	case 3:
		_BloodObj->Animator2D()->Play(L"BloodLeft2", false);
		break;
	case 4:
		_BloodObj->Animator2D()->Play(L"BloodLeftUp", false);
		break;
	case 5:
		_BloodObj->Animator2D()->Play(L"BloodUpLeft", false);
		break;
	case 10:
		_BloodObj->Animator2D()->Play(L"BloodRightDown", false);
		break;
	case 11:
		_BloodObj->Animator2D()->Play(L"BloodDownRight", false);
		break;
	case 12:
		_BloodObj->Animator2D()->Play(L"BloodRight1", false);
		break;
	case 13:
		_BloodObj->Animator2D()->Play(L"BloodRight2", false);
		break;
	case 14:
		_BloodObj->Animator2D()->Play(L"BloodRightUp", false);
		break;
	case 15:
		_BloodObj->Animator2D()->Play(L"BloodUpRight", false);
		break;
	}
}

void CEffectScript::CreateBlood()
{
	for (int i = 0; i < 6; ++i)
	{
		wchar_t number[64] = {};
		swprintf_s(number, L"blood%d", i);
		CGameObject* pBlood = m_BloodPrefab->Instantiate();
		pBlood->SetName(number);
		GamePlayStatic::SpawnGameObject(pBlood, 5);
		pBlood->Transform()->SetRelativePos(Vec3(m_vCalculatedPos.x, m_vCalculatedPos.y, 0.f));
		pBlood->Animator2D()->Play(L"BloodNormal", true);

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

