#include "pch.h"
#include "CRunningState.h"

#include <Engine/components.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>

CRunningState::CRunningState()
	:m_Dir(eDIR::NONE)
	, m_Anim(nullptr)
{
}

CRunningState::~CRunningState()
{
}

void CRunningState::finaltick()
{
	float Speed = *((float*)GetBlackboardData(L"Speed"));
	Vec3 vPos = GetFSM()->GetStateMachine()->Transform()->GetRelativePos();

	if (KEY_PRESSED(KEY::W))
	{
		vPos.y += DT * Speed;
	}
	if (KEY_TAP(KEY::W) && KEY_NONE(KEY::A) && KEY_NONE(KEY::S) && KEY_NONE(KEY::D))
	{
		m_Dir = eDIR::UP;
		ChangeState(L"RunningState");
	}
	if (KEY_RELEASED(KEY::W))
	{
		if (KEY_NONE(KEY::D) && KEY_NONE(KEY::A))
		{
			m_Anim->Play(L"Stand_Up");
			m_Dir = eDIR::UP;
		}
	}

	if (KEY_PRESSED(KEY::S))
	{
		vPos.y -= DT * Speed;

	}
	if (KEY_TAP(KEY::S) && KEY_NONE(KEY::W) && KEY_NONE(KEY::A) && KEY_NONE(KEY::D))
	{
		m_Dir = eDIR::DOWN;
		ChangeState(L"RunningState");
	}
	if (KEY_RELEASED(KEY::S))
	{
		if (KEY_NONE(KEY::D) && KEY_NONE(KEY::A))
		{
			m_Anim->Play(L"Stand_Down");
			m_Dir = eDIR::DOWN;

		}
	}

	if (KEY_PRESSED(KEY::A))
	{
		vPos.x -= DT * Speed;
		if (KEY_TAP(KEY::W))
		{
			m_Dir = eDIR::UP;
			ChangeState(L"RunningState");
		}

		if (KEY_TAP(KEY::S))
		{
			m_Dir = eDIR::DOWN;
			ChangeState(L"RunningState");
		}

		if (KEY_TAP(KEY::D))
		{
			m_Anim->Play(L"Stand_Left");
			m_Dir = eDIR::LEFT;

		}

		if (KEY_RELEASED(KEY::D))
		{
			m_Dir = eDIR::LEFT;
			ChangeState(L"RunningState");
		}

		if (KEY_RELEASED(KEY::W) || KEY_RELEASED(KEY::S))
		{
			m_Dir = eDIR::LEFT;
			ChangeState(L"RunningState");
		}
	}
	if (KEY_TAP(KEY::A) && KEY_NONE(KEY::W) && KEY_NONE(KEY::S) && KEY_NONE(KEY::D))
	{
		m_Dir = eDIR::LEFT;
		ChangeState(L"RunningState");
	}
	if (KEY_RELEASED(KEY::A))
	{
		if (KEY_NONE(KEY::W) && KEY_NONE(KEY::S))
		{
			m_Anim->Play(L"Stand_Left");
			m_Dir = eDIR::LEFT;

		}
	}

	if (KEY_PRESSED(KEY::D))
	{
		vPos.x += DT * Speed;

		if (KEY_TAP(KEY::W))
		{
			m_Dir = eDIR::UP;
			ChangeState(L"RunningState");
		}

		if (KEY_TAP(KEY::S))
		{
			m_Dir = eDIR::DOWN;
			ChangeState(L"RunningState");
		}

		if (KEY_TAP(KEY::A))
		{
			m_Anim->Play(L"Stand_Right");
			m_Dir = eDIR::RIGHT;

		}

		if (KEY_RELEASED(KEY::A))
		{
			m_Dir = eDIR::RIGHT;
			ChangeState(L"RunningState");
		}

		if (KEY_RELEASED(KEY::W) || KEY_RELEASED(KEY::S))
		{
			m_Dir = eDIR::RIGHT;
			ChangeState(L"RunningState");
		}
	}
	if (KEY_TAP(KEY::D) && KEY_NONE(KEY::W) && KEY_NONE(KEY::S) && KEY_NONE(KEY::A))
	{
		m_Dir = eDIR::RIGHT;
		ChangeState(L"RunningState");
	}
	if (KEY_RELEASED(KEY::D))
	{
		if (KEY_NONE(KEY::W) && KEY_NONE(KEY::S) && KEY_NONE(KEY::A))
		{
			m_Anim->Play(L"Stand_Right");
			m_Dir = eDIR::RIGHT;
		}
	}

	if (KEY_NONE(KEY::W) && KEY_NONE(KEY::S) && KEY_NONE(KEY::A) && KEY_NONE(KEY::D))
	{
		ChangeState(L"StandState");
	}

	GetFSM()->GetStateMachine()->Transform()->SetRelativePos(vPos);
}

void CRunningState::Enter()
{
	m_Dir = *((eDIR*)GetBlackboardData(L"Dir"));
	m_Anim = GetFSM()->GetStateMachine()->Animator2D();

	switch (m_Dir)
	{	
	case eDIR::UP:
		m_Anim->Play(L"Running_Up");
		break;
	case eDIR::DOWN:
		m_Anim->Play(L"Running_Down");
		break;
	case eDIR::LEFT:
		m_Anim->Play(L"Running_Left");
		break;
	case eDIR::RIGHT:
		m_Anim->Play(L"Running_Right2");
		break;
	case eDIR::NONE:
		break;
	default:
		break;
	}
}

void CRunningState::Exit()
{
	SetBlackboardData(L"Dir", &m_Dir);
}

