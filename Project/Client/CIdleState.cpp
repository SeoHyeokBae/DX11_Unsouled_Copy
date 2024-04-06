#include "pch.h"
#include "CIdleState.h"

#include <Engine/components.h>
#include <Engine/CGameObject.h>
#include <Engine/CStateMachine.h>
#include <Engine/CTimeMgr.h>

CIdleState::CIdleState()
{
}

CIdleState::~CIdleState()
{
}

void CIdleState::finaltick()
{
	// 플레이어가 근처에 있으면, Trace 상태로 변경한다.
	float DetectRange = *((float*)GetBlackboardData(L"DetectRange"));
	CGameObject* pTarget = ((CGameObject*)GetBlackboardData(L"TargetObject"));

	CGameObject* pSelf = GetFSM()->GetStateMachine()->GetOwner();

	Vec3 vDist = pTarget->Transform()->GetWorldPos() - pSelf->Transform()->GetWorldPos();

	GamePlayStatic::DrawDebugCircle(pSelf->Transform()->GetWorldPos(), DetectRange, Vec3(0.1f, 1.f, 0.1f), 0.f);

	if (vDist.Length() <= DetectRange)
	{
		// 추적상태로 변경한다.
		ChangeState(L"TraceState");
	}
}

void CIdleState::Enter()
{
	m_Dir = GetFSM()->GetStateMachine()->GetOwner()->GetDir();
	CAnimator2D* anim = GetFSM()->GetStateMachine()->Animator2D();

	// 나중에 몬스터 애니메이션 이름 통일
	switch (m_Dir)
	{
	case eDIR::UP:
		anim->Play(L"Stand_Right", true);
		break;
	case eDIR::DOWN:
		anim->Play(L"Stand_Left", true);
		break;
	case eDIR::LEFT:
		anim->Play(L"Stand_Left", true);
		break;
	case eDIR::RIGHT:
		anim->Play(L"Stand_Right", true);
		break;
	case eDIR::UPLEFT:
		anim->Play(L"Stand_Left", true);
		break;
	case eDIR::UPRIGHT:
		anim->Play(L"Stand_Right", true);
		break;
	case eDIR::DOWNLEFT:
		anim->Play(L"Stand_Left", true);
		break;
	case eDIR::DOWNRIGHT:
		anim->Play(L"Stand_Right", true);
		break;
	}
}

void CIdleState::Exit()
{

}