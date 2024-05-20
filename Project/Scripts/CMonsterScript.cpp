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
		if (_OtherObj->GetParent()->GetScript<CPlayerScript>()->IsHit())
		{
			// ㅂ보스제외 좀비만 테스트 임시
			if (GetOwner()->GetScript<CBossNiugScript>())
				return;

			// Player -> Player_AttCol 로 변경
			Vec2 vPos = GetOwner()->Transform()->GetRelativePos().XY() + _Collider->GetOffsetPos();	// 몬스터 위치
			Vec2 vSize = _Collider->GetOffsetScale();	// col 사이즈
			Vec2 vOtherPos = _OtherObj->GetParent()->Transform()->GetRelativePos().XY() + _OtherCollider->GetOffsetPos(); // Player_Attcol Pos
			Vec2 vOtherSize = _OtherCollider->GetOffsetScale();	// Player_AttCol size
			Vec2 vPosLength = vSize / 2.f;
			Vec2 vOtherLength = vOtherSize / 2.f;

			Vector2 vEffectPos = Vec2(0.f, 0.f);
			
			// 몬스터로 향하는 방향 벡터
			// 몬스터별로 적용
			// 위치 좀더 몬스터쪽으로 조정 필요
			// 이펙트 출력 layer 우선순위 조정필요

			Vec2 vdir = vPos - vOtherPos;
			vdir.Normalize(); 
			
			// 검끝 기준
			if (vdir.x < 0) // attcol 우측
			{
				//vEffectPos.x = vOtherPos.x - vOtherLength.x + (vPos.x - (vOtherPos.x - vOtherLength.x)) / 2.f;
				vEffectPos.x = vOtherPos.x - vOtherSize.x;
			}
			else if (vdir.x >= 0) //attcol 좌측
			{
				//vEffectPos.x = vOtherPos.x + vOtherLength.x - (vOtherPos.x + vOtherLength.x - vPos.x) / 2.f;
				vEffectPos.x = vOtherPos.x + vOtherSize.x;
			}

			if (vdir.y < 0) // attcol 위
			{
				//vEffectPos.y  = vOtherPos.y - vOtherLength.y + (vPos.y - (vOtherPos.y - vOtherLength.y)) / 2.f;
				vEffectPos.y = vOtherPos.y - vOtherSize.y;
			}
			else if (vdir.y >= 0) //attcol 아래
			{
				//vEffectPos.y = vOtherPos.y + vOtherLength.y - (vOtherPos.y + vOtherLength.y - vPos.y) / 2.f;
				vEffectPos.y = vOtherPos.y + vOtherSize.y;
			}


			GetOwner()->GetScript<CEffectScript>()->SetCalculatedPos(vEffectPos);

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

