#include "pch.h"
#include "CDashState.h"

#include <Engine/components.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>

CDashState::CDashState()
{
}

CDashState::~CDashState()
{
}

void CDashState::finaltick()
{


	if (/*RECOVERYTIME <= m_fTiming &&*/ KEY_NONE(KEY::RBTN))
	{
		//m_fTiming = 0.f;
		ChangeState(L"StandState");
	}
}

void CDashState::Enter()
{
	m_Dir = GetFSM()->GetStateMachine()->GetOwner()->GetDir();
	CAnimator2D* anim = GetFSM()->GetStateMachine()->Animator2D();
	Vec2 vForce = Vec2(0.f, 0.f);

	switch (m_Dir)
	{
	case eDIR::UP:
		anim->Play(L"Dash_Up", false);
		vForce.y = 2000.f;
		break;
	case eDIR::DOWN:
		anim->Play(L"Dash_Down", false);
		vForce.y = -2000.f;
		break;
	case eDIR::LEFT:
		anim->Play(L"Dash_Left", false);
		vForce.x = -2000.f;
		break;
	case eDIR::RIGHT:
		anim->Play(L"Dash_Right", false);
		vForce.x = 2000.f;
		break;
	case eDIR::UPLEFT:
		anim->Play(L"Dash_Up", false);
		vForce.x = -2000.f; vForce.y = 2000.f;
		break;
	case eDIR::UPRIGHT:
		anim->Play(L"Dash_UpRight", false);
		vForce.x = 2000.f; vForce.y = 2000.f;
		break;
	case eDIR::DOWNLEFT:
		anim->Play(L"Dash_Down", false);
		vForce.x = -2000.f; vForce.y = -2000.f;
		break;
	case eDIR::DOWNRIGHT:
		anim->Play(L"Dash_DownRight", false);
		vForce.x = 2000.f; vForce.y = -2000.f;
		break;
	}

	GetFSM()->GetStateMachine()->Movement()->AddForce(vForce);
}

void CDashState::Exit()
{
	//m_bStart = false;
	//SetBlackboardData(L"Chain", &m_bStart);
	GetFSM()->GetStateMachine()->GetOwner()->SetDir(m_Dir);
}

