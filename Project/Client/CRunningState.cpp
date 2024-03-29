#include "pch.h"
#include "CRunningState.h"

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
	CAnimator2D* anim = GetFSM()->GetStateMachine()->Animator2D();

	if (KEY_PRESSED(KEY::W))
	{
		vPos.y += DT * Speed;
	}
	if (KEY_TAP(KEY::W) && KEY_CHECK(KEY::A, NONE) && KEY_CHECK(KEY::S, NONE) && KEY_CHECK(KEY::D, NONE))
	{
		anim->Play(L"Running_Up");
	}
	if (KEY_RELEASED(KEY::W))
	{
		if (KEY_CHECK(KEY::D, NONE) && KEY_CHECK(KEY::A, NONE))
			anim->Play(L"Stand_Up");
	}


	if (KEY_PRESSED(KEY::S))
	{
		vPos.y -= DT * Speed;

	}
	if (KEY_TAP(KEY::S) && KEY_CHECK(KEY::W, NONE) && KEY_CHECK(KEY::A, NONE) && KEY_CHECK(KEY::D, NONE))
	{
		anim->Play(L"Running_Down");
	}
	if (KEY_RELEASED(KEY::S))
	{
		if (KEY_CHECK(KEY::D, NONE) && KEY_CHECK(KEY::A, NONE))
			anim->Play(L"Stand_Down");
	}


	if (KEY_PRESSED(KEY::A))
	{
		vPos.x -= DT * Speed;
		if (KEY_TAP(KEY::W))
		{
			anim->Play(L"Running_Up");
		}

		if (KEY_TAP(KEY::S))
		{
			anim->Play(L"Running_Down");
		}

		if (KEY_TAP(KEY::D))
		{
			anim->Play(L"Stand_Left");
		}

		if (KEY_RELEASED(KEY::D))
		{
			anim->Play(L"Running_Left");
		}

		if (KEY_RELEASED(KEY::W) || KEY_RELEASED(KEY::S))
		{
			anim->Play(L"Running_Left");
		}
	}
	if (KEY_TAP(KEY::A) && KEY_CHECK(KEY::W, NONE) && KEY_CHECK(KEY::S, NONE) && KEY_CHECK(KEY::D, NONE))
	{
		anim->Play(L"Running_Left");
	}
	if (KEY_RELEASED(KEY::A))
	{
		if (KEY_CHECK(KEY::W, NONE) && KEY_CHECK(KEY::S, NONE))
			anim->Play(L"Stand_Left");
	}

	if (KEY_PRESSED(KEY::D))
	{
		vPos.x += DT * Speed;

		if (KEY_TAP(KEY::W))
		{
			anim->Play(L"Running_Up");
		}

		if (KEY_TAP(KEY::S))
		{
			anim->Play(L"Running_Down");
		}

		if (KEY_TAP(KEY::A))
		{
			anim->Play(L"Stand_Right");
		}

		if (KEY_RELEASED(KEY::A))
		{
			anim->Play(L"Running_Right2");
		}

		if (KEY_RELEASED(KEY::W) || KEY_RELEASED(KEY::S))
		{
			anim->Play(L"Running_Right2");
		}
	}
	if (KEY_TAP(KEY::D) && KEY_CHECK(KEY::W, NONE) && KEY_CHECK(KEY::S, NONE) && KEY_CHECK(KEY::A, NONE))
	{
		anim->Play(L"Running_Right2");
	}
	if (KEY_RELEASED(KEY::D))
	{
		if (KEY_CHECK(KEY::W, NONE) && KEY_CHECK(KEY::S, NONE) && KEY_CHECK(KEY::S, RELEASED))
			anim->Play(L"Stand_Right");
	}

	GetFSM()->GetStateMachine()->Transform()->SetRelativePos(vPos);
}

void CRunningState::Enter()
{
}

void CRunningState::Exit()
{
}

