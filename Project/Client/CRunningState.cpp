#include "pch.h"
#include "CRunningState.h"

#include <Scripts/CPlayerScript.h>
#include <Engine/components.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>

CRunningState::CRunningState()
{
}

CRunningState::~CRunningState()
{
}

void CRunningState::finaltick()
{
	float Speed = *((float*)GetBlackboardData(L"Speed"));
	Vec3 vPos = GetFSM()->GetStateMachine()->Transform()->GetRelativePos();
}

void CRunningState::Enter()
{
	eDIR dir = GetFSM()->GetStateMachine()->GetOwner()->GetScript<CPlayerScript>()->GetDir();
	CAnimator2D* anim = GetFSM()->GetStateMachine()->Animator2D();

	switch (dir)
	{	
	case eDIR::UP:
		anim->Play(L"Running_Up");
		break;
	case eDIR::DOWN:
		anim->Play(L"Running_Down");
		break;
	case eDIR::LEFT:
		anim->Play(L"Running_Left");
		break;
	case eDIR::RIGHT:
		anim->Play(L"Running_Right2");
		break;
	case eDIR::NONE:
		break;
	default:
		break;
	}
}

void CRunningState::Exit()
{
}

