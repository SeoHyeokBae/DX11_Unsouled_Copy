#include "pch.h"
#include "CStandState.h"

#include <Engine/components.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>

CStandState::CStandState()
{
}

CStandState::~CStandState()
{
}

void CStandState::finaltick()
{
	if (!KEY_NONE(KEY::D) || !KEY_NONE(KEY::S) || !KEY_NONE(KEY::A) || !KEY_NONE(KEY::W))
	{
		ChangeState(L"RunningState");
	}
}

void CStandState::Enter()
{
}

void CStandState::Exit()
{
	CAnimator2D* anim = GetFSM()->GetStateMachine()->Animator2D();

	if (KEY_TAP(KEY::W)) anim->Play(L"Running_Up");
	if (KEY_TAP(KEY::S)) anim->Play(L"Running_Down");
	if (KEY_TAP(KEY::A)) anim->Play(L"Running_Left");
	if (KEY_TAP(KEY::D)) anim->Play(L"Running_Right2");
}
