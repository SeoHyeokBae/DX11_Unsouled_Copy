#include "pch.h"
#include "CStaminaOutState.h"

#include <Engine/components.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>


CStaminaOutState::CStaminaOutState()
	: m_bUP(true)
	, m_bRight(true)
{
}

CStaminaOutState::~CStaminaOutState()
{
}

void CStaminaOutState::finaltick()
{
	float Speed = 20.f;
	Vec3 vPos = GetFSM()->GetStateMachine()->Transform()->GetRelativePos();
	CAnimator2D* anim = GetFSM()->GetStateMachine()->Animator2D();

	if (KEY_PRESSED(KEY::W))
	{
		vPos.y += DT * Speed;
		if (KEY_TAP(KEY::D))
		{	
			anim->Play(L"StaminaOut_Walking_Up_Right");
		}

		if (KEY_TAP(KEY::A))
		{
			anim->Play(L"StaminaOut_Walking_Up_Left");
		}

		if (KEY_RELEASED(KEY::A))
		{
			anim->Play(L"StaminaOut_Walking_Up_Left");
		}
	}
	if (KEY_TAP(KEY::W) && KEY_CHECK(KEY::A, NONE) && KEY_CHECK(KEY::S, NONE) && KEY_CHECK(KEY::D, NONE))
	{
		anim->Play(L"StaminaOut_Walking_Up_Left");
	}
	if (KEY_RELEASED(KEY::W))
	{
		if (KEY_CHECK(KEY::D, NONE) && KEY_CHECK(KEY::A, NONE))
			anim->Play(L"StaminaOut_Walking_Up_Stand");
	}


	if (KEY_PRESSED(KEY::S))
	{
		vPos.y -= DT * Speed;
		if (KEY_TAP(KEY::D))
		{
			anim->Play(L"StaminaOut_Walking_Down_Right");
		}

		if (KEY_TAP(KEY::A))
		{
			anim->Play(L"StaminaOut_Walking_Down_Left");
		}

		if (KEY_TAP(KEY::D))
		{
			anim->Play(L"StaminaOut_Walking_Down_Right");
		}

	}
	if (KEY_TAP(KEY::S) && KEY_CHECK(KEY::W, NONE) && KEY_CHECK(KEY::A, NONE) && KEY_CHECK(KEY::D, NONE))
	{
		anim->Play(L"StaminaOut_Walking_Down_Right");
	}
	if (KEY_RELEASED(KEY::S))
	{
		if (KEY_CHECK(KEY::D, NONE) && KEY_CHECK(KEY::A, NONE))
			anim->Play(L"StaminaOut_Walking_Down_Stand");
	}


	if (KEY_PRESSED(KEY::A))
	{
		vPos.x -= DT * Speed;
		if (KEY_TAP(KEY::W))
		{
			anim->Play(L"StaminaOut_Walking_Up_Left");
		}

		if (KEY_TAP(KEY::S))
		{
			anim->Play(L"StaminaOut_Walking_Down_Left");
		}

		if (KEY_RELEASED(KEY::W))
		{
			anim->Play(L"StaminaOut_Walking_Down_Left");
		}
	}
	if (KEY_TAP(KEY::A) && KEY_CHECK(KEY::W, NONE) && KEY_CHECK(KEY::S, NONE) && KEY_CHECK(KEY::D, NONE))
	{
		anim->Play(L"StaminaOut_Walking_Down_Stand");
	}
	if (KEY_RELEASED(KEY::A))
	{
		if (KEY_CHECK(KEY::W, NONE) && KEY_CHECK(KEY::S, NONE))
			anim->Play(L"StaminaOut_Walking_Down_Stand");
	}


	if (KEY_PRESSED(KEY::D))
	{
		vPos.x += DT * Speed;
		if (KEY_TAP(KEY::W))
		{
			anim->Play(L"StaminaOut_Walking_Up_Right");
		}

		if (KEY_TAP(KEY::S))
		{
			anim->Play(L"StaminaOut_Walking_Down_Right");
		}

		if (KEY_RELEASED(KEY::S))
		{
			anim->Play(L"StaminaOut_Walking_Up_Right");
		}
	}
	if (KEY_TAP(KEY::D) && KEY_CHECK(KEY::W, NONE) && KEY_CHECK(KEY::S, NONE) && KEY_CHECK(KEY::A, NONE))
	{
		anim->Play(L"StaminaOut_Walking_Up_Stand");
	}
	if (KEY_RELEASED(KEY::D))
	{
		if (KEY_CHECK(KEY::W, NONE) && KEY_CHECK(KEY::S, NONE))
			anim->Play(L"StaminaOut_Walking_Up_Stand");
	}

	GetFSM()->GetStateMachine()->Transform()->SetRelativePos(vPos);
}

void CStaminaOutState::Enter()
{
}

void CStaminaOutState::Exit()
{
}
