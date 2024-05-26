#include "pch.h"
#include "CMonsterScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

#include "CPlayerScript.h"
#include "CEffectScript.h"
#include "CZombieScript.h"
#include "CBossNiugScript.h"

#include <Scripts/CAfterImageScript.h>

#include "../Client/CIdleState.h"
#include "../Client/CTraceState.h"

CMonsterScript::CMonsterScript()
	: CScript(MONSTERSCRIPT)
	, m_DetectRange(200.f)
	, m_Speed(100.f)
	, m_AftTime(0.f)
{

}

CMonsterScript::~CMonsterScript()
{
}

void CMonsterScript::begin()
{
	// 동적재질로 변경
	GetRenderComponent()->GetDynamicMaterial();

	if (GetOwner()->GetName() == L"Zomibie" || GetOwner()->GetName() == L"Room_Zomibie")
	{
		m_Speed = 50.f;
		m_DetectRange = 200.f;
	}
	else if (GetOwner()->GetName() == L"Boss_Niug")
	{
		m_Speed = 75.f;
		m_DetectRange = 400.f;
	}

	StateMachine()->AddBlackboardData(L"DetectRange", BB_DATA::FLOAT, &m_DetectRange);
	StateMachine()->AddBlackboardData(L"Speed", BB_DATA::FLOAT, &m_Speed);

	// 플레이어를 블랙보드에 등록
	CGameObject* pPlayer = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Player");
	if (pPlayer)
	{
		StateMachine()->AddBlackboardData(L"TargetObject", BB_DATA::OBJECT, pPlayer);
	}
}

void CMonsterScript::tick()
{
	// 잔상 효과 이미지 정보 저장
	if (GetOwner()->IsAfterImgAct())
	{
		CreateAftImg();

		if (Animator2D()->GetCurAnim()->IsFinish())
		{
			GetOwner()->SetAfterImgAct(false);
		}
	}
}

void CMonsterScript::CreateAftImg()
{
	// 잔상 스크립트 없음
	assert(GetOwner()->GetScript<CAfterImageScript>());

	Vec3 vPos = Transform()->GetRelativePos();

	if (m_AftTime == 0.f)
	{
		int idx = GetOwner()->GetScript<CAfterImageScript>()->GetCurIdx();
		if (idx % 3 < 3)
		{
			int animidx = Animator2D()->GetCurAnim()->GetCurFrmIdx();

			tAnimFrm frm = Animator2D()->GetCurAnim()->GetCurFrmInfo(animidx);
			Ptr<CTexture> tex = Animator2D()->GetCurAnim()->GetAtalsTex();

			GetOwner()->GetScript<CAfterImageScript>()->CreateAfterImg(tex, vPos, frm);

			idx++;
			GetOwner()->GetScript<CAfterImageScript>()->SetCurIdx(idx);
		}
	}

	m_AftTime += DT;
	if (m_AftTime >= 0.06f)
	{
		m_AftTime = 0.f;
		int idx = GetOwner()->GetScript<CAfterImageScript>()->GetCurIdx();
		if (idx >= 50)
			return;

		int animidx = Animator2D()->GetCurAnim()->GetCurFrmIdx();

		tAnimFrm frm = Animator2D()->GetCurAnim()->GetCurFrmInfo(animidx);
		Ptr<CTexture> tex = Animator2D()->GetCurAnim()->GetAtalsTex();

		GetOwner()->GetScript<CAfterImageScript>()->CreateAfterImg(tex, vPos, frm);

		idx++;
		GetOwner()->GetScript<CAfterImageScript>()->SetCurIdx(idx);
	}
}


void CMonsterScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{

}

void CMonsterScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{	

	if (_OtherObj->GetName() == L"Player_AttCol")
	{
		if (_OtherObj->GetParent()->GetScript<CPlayerScript>()->IsCanHit())
		{
			// ㅂ보스제외 좀비만 테스트 임시
			if (GetOwner()->GetScript<CBossNiugScript>())
				return;

			Vec2 vPos = GetOwner()->Transform()->GetRelativePos().XY() + _Collider->GetOffsetPos();	// 몬스터 collider 위치
			Vec2 vOtherPos = _OtherObj->GetParent()->Transform()->GetRelativePos().XY() + _OtherCollider->GetOffsetPos(); // Player_Attcol Pos

			Vector2 vEffectPos = Vec2(0.f, 0.f);
			Vector3 vEffectRot = Vec3(0.f, 0.f,0.f);
		
			vEffectPos = vPos; // 몬스터OBJ 위치

			vPos = GetOwner()->Transform()->GetRelativePos().XY();
			Vec2 vDir = vOtherPos - vPos; // 타켓으로부터 방향벡터
			vDir.Normalize();

			// EffectRot y축은 항상 vOtherPos을 가리킴
			float angle = acos(vDir.Dot(Vec2(0.0f, 1.f)) / vDir.Length());
			if (0.f < vDir.x)
			{
				vEffectRot = Vec3(0.f, 0.f, -angle); // 왼
				if (vOtherPos.y < vPos.y)
					GetOwner()->GetScript<CEffectScript>()->SetBloodDir(eBloodDir::LEFTDOWN);
				else
					GetOwner()->GetScript<CEffectScript>()->SetBloodDir(eBloodDir::LEFTUP);
			}
			else
			{
				vEffectRot = Vec3(0.f, 0.f, angle);	 // 오
				if (vOtherPos.y < vPos.y)
					GetOwner()->GetScript<CEffectScript>()->SetBloodDir(eBloodDir::RIGHTDOWN);
				else
					GetOwner()->GetScript<CEffectScript>()->SetBloodDir(eBloodDir::RIGHTUP);
			}
			GetOwner()->GetScript<CEffectScript>()->SetCalculatedPos(vEffectPos);
			GetOwner()->GetScript<CEffectScript>()->SetCalculatedRot(vEffectRot);

			// 좀비일때
			if (GetOwner()->GetScript<CZombieScript>()) 
			{
				GetOwner()->GetScript<CZombieScript>()->Damaged();
			}

			// 보스일때
			else if (GetOwner()->GetScript<CBossNiugScript>())
			{
				//To Do
			}
			
		}
	}
	

}

void CMonsterScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CMonsterScript::SaveToFile(FILE* _File)
{
}

void CMonsterScript::LoadFromFile(FILE* _File)
{
}

void CMonsterScript::init()
{
}

