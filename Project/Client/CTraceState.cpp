#include "pch.h"
#include "CTraceState.h"

#include <Engine/components.h>
#include <Engine/CGameObject.h>
#include <Engine/CTimeMgr.h>

CTraceState::CTraceState()
{
}

CTraceState::~CTraceState()
{
}


void CTraceState::finaltick()
{
	// 플레이어가 근처에 있으면, Trace 상태로 변경한다.	
	float Speed = *((float*)GetBlackboardData(L"Speed"));
	CGameObject* pTarget = ((CGameObject*)GetBlackboardData(L"TargetObject"));
	CGameObject* pSelf = GetFSM()->GetStateMachine()->GetOwner();

	Vec3 vDir = pTarget->Transform()->GetWorldPos() - pSelf->Transform()->GetWorldPos();
	vDir.Normalize();

	//Vec3 vPos = pSelf->Transform()->GetWorldPos() + vDir * DT * Speed;
	//Vec3 vPos = vDir * DT * Speed;
	//pSelf->Transform()->SetRelativePos(vPos);
	Vec3 vPos = vDir * Speed;
	//pSelf->Movement()->SetVelocity(vPos.XY());
}

void CTraceState::Enter()
{
	m_Dir = GetFSM()->GetStateMachine()->GetOwner()->GetDir();
	CAnimator2D* anim = GetFSM()->GetStateMachine()->Animator2D();

	switch (m_Dir)
	{
	case eDIR::UP:
		anim->Play(L"Running_Right", true);
		break;
	case eDIR::DOWN:
		anim->Play(L"Running_Left", true);
		break;
	case eDIR::LEFT:
		anim->Play(L"Running_Left", true);
		break;
	case eDIR::RIGHT:
		anim->Play(L"Running_Right", true);
		break;
	case eDIR::UPLEFT:
		anim->Play(L"Running_Left", true);
		break;
	case eDIR::UPRIGHT:
		anim->Play(L"Running_Right", true);
		break;
	case eDIR::DOWNLEFT:
		anim->Play(L"Running_Left", true);
		break;
	case eDIR::DOWNRIGHT:
		anim->Play(L"Running_Right", true);
		break;
	}
}

void CTraceState::Exit()
{
}