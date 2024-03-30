#include "pch.h"
#include "CAbsorbState.h"

#include <Engine/components.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>

CAbsorbState::CAbsorbState()
	:m_Anim(nullptr)
{
}

CAbsorbState::~CAbsorbState()
{
}

void CAbsorbState::finaltick()
{
	if (KEY_NONE(KEY::F))
	{
		ChangeState(L"StandState");
	}

	float Speed = 20.f;
	Vec3 vPos = GetFSM()->GetStateMachine()->Transform()->GetRelativePos();

	if (KEY_PRESSED(KEY::W))
	{
		vPos.y += DT * Speed;
		if (KEY_TAP(KEY::D))
		{
			m_Dir = eDIR::RIGHT;
			m_Anim->Play(L"Absorb_Walking_Right");
		}

		if (KEY_TAP(KEY::A))
		{
			m_Dir = eDIR::LEFT;
			m_Anim->Play(L"Absorb_Walking_Left");
		}

		if (KEY_RELEASED(KEY::A) || KEY_RELEASED(KEY::D))
		{
			m_Dir = eDIR::UP;
			m_Anim->Play(L"Absorb_Walking_Up");
		}
	}
	if (KEY_TAP(KEY::W) && KEY_NONE(KEY::A) && KEY_NONE(KEY::S) && KEY_NONE(KEY::D))
	{
		m_Dir = eDIR::UP;
		m_Anim->Play(L"Absorb_Walking_Up");
	}
	if (KEY_RELEASED(KEY::W))
	{
		if (KEY_NONE(KEY::D) && KEY_NONE(KEY::A))
		{
			m_Dir = eDIR::UP;
			m_Anim->Play(L"Absorb_Stand_Up");
		}
	}


	if (KEY_PRESSED(KEY::S))
	{
		vPos.y -= DT * Speed;
		if (KEY_TAP(KEY::D))
		{
			m_Dir = eDIR::RIGHT;
			m_Anim->Play(L"Absorb_Walking_Right");
		}

		if (KEY_TAP(KEY::A))
		{
			m_Dir = eDIR::LEFT;
			m_Anim->Play(L"Absorb_Walking_Left");
		}

		if (KEY_RELEASED(KEY::D) || KEY_RELEASED(KEY::A))
		{
			m_Dir = eDIR::DOWN;
			m_Anim->Play(L"Absorb_Walking_Down");
		}

	}
	if (KEY_TAP(KEY::S) && KEY_NONE(KEY::W) && KEY_NONE(KEY::A) && KEY_NONE(KEY::D))
	{
		m_Dir = eDIR::DOWN;
		m_Anim->Play(L"Absorb_Walking_Down");
	}
	if (KEY_RELEASED(KEY::S))
	{
		m_Dir = eDIR::DOWN;
		if (KEY_NONE(KEY::D) && KEY_NONE(KEY::A))
			m_Anim->Play(L"Absorb_Stand_Down");
	}


	if (KEY_PRESSED(KEY::A))
	{
		vPos.x -= DT * Speed;
		if (KEY_TAP(KEY::D))
		{
			m_Dir = eDIR::LEFT;
			m_Anim->Play(L"Absorb_Stand_Left");
		}

		if (KEY_RELEASED(KEY::D))
		{
			m_Dir = eDIR::LEFT;
			m_Anim->Play(L"Absorb_Walking_Left");
		}
	}
	if (KEY_TAP(KEY::A) && KEY_NONE(KEY::D))
	{
		m_Dir = eDIR::LEFT;
		m_Anim->Play(L"Absorb_Walking_Left");
	}
	if (KEY_RELEASED(KEY::A))
	{
		if (KEY_NONE(KEY::W) && KEY_NONE(KEY::S) && KEY_NONE(KEY::D))
		{
			m_Dir = eDIR::LEFT;
			m_Anim->Play(L"Absorb_Stand_Left");
		}
	}


	if (KEY_PRESSED(KEY::D))
	{
		vPos.x += DT * Speed;
		if (KEY_TAP(KEY::A))
		{
			m_Dir = eDIR::RIGHT;
			m_Anim->Play(L"Absorb_Stand_Right");
		}

		if (KEY_RELEASED(KEY::A))
		{
			m_Dir = eDIR::RIGHT;
			m_Anim->Play(L"Absorb_Walking_Right");
		}
	}
	if (KEY_TAP(KEY::D) && KEY_NONE(KEY::A))
	{
		m_Dir = eDIR::RIGHT;
		m_Anim->Play(L"Absorb_Walking_Right");
	}
	if (KEY_RELEASED(KEY::D))
	{
		if (KEY_NONE(KEY::W) && KEY_NONE(KEY::S) && KEY_NONE(KEY::A))
		{
			m_Dir = eDIR::RIGHT;
			m_Anim->Play(L"Absorb_Stand_Right");
		}
	}

	GetFSM()->GetStateMachine()->Transform()->SetRelativePos(vPos);
}

void CAbsorbState::Enter()
{
	m_Dir = *((eDIR*)GetBlackboardData(L"Dir"));
	m_Anim = GetFSM()->GetStateMachine()->Animator2D();

	switch (m_Dir)
	{
	case eDIR::UP:
		KEY_PRESSED(KEY::W) ? m_Anim->Play(L"Absorb_Walking_Up") : m_Anim->Play(L"Absorb_Stand_Up");
		break;
	case eDIR::DOWN:
		KEY_PRESSED(KEY::S) ? m_Anim->Play(L"Absorb_Walking_Down") : m_Anim->Play(L"Absorb_Stand_Down");
		break;
	case eDIR::LEFT:
		KEY_PRESSED(KEY::A) ? m_Anim->Play(L"Absorb_Walking_Left") : m_Anim->Play(L"Absorb_Stand_Left");
		break;
	case eDIR::RIGHT:
		KEY_PRESSED(KEY::D) ? m_Anim->Play(L"Absorb_Walking_Right") : m_Anim->Play(L"Absorb_Stand_Right");
		break;
	case eDIR::NONE:
		break;
	default:
		break;
	}
}

void CAbsorbState::Exit()
{
	SetBlackboardData(L"Dir", &m_Dir);
}


