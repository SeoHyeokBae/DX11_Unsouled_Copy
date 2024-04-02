#include "pch.h"
#include "CPlayerBlockState.h"

#include <Engine/components.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>

CPlayerBlockState::CPlayerBlockState()
	: m_Anim(nullptr)
{
}

CPlayerBlockState::~CPlayerBlockState()
{
}

void CPlayerBlockState::finaltick()
{
	// TODO

	if (KEY_PRESSED(KEY::W) || KEY_PRESSED(KEY::A) || KEY_PRESSED(KEY::S) || KEY_PRESSED(KEY::D))
	{
		if (KEY_TAP(KEY::W) || KEY_TAP(KEY::A) || KEY_TAP(KEY::S) || KEY_TAP(KEY::D))
		{
			ChangeState(L"BlockState");
		}

		if (KEY_RELEASED(KEY::W) || KEY_RELEASED(KEY::A) || KEY_RELEASED(KEY::S) || KEY_RELEASED(KEY::D))
		{
			ChangeState(L"BlockState");
		}
	}

	if ((KEY_TAP(KEY::W) || KEY_PRESSED(KEY::W)) && KEY_NONE(KEY::A) && KEY_NONE(KEY::S))
	{
		m_Dir = eDIR::UP;
		ChangeState(L"BlockState");
	}
	if ((KEY_TAP(KEY::A) || KEY_PRESSED(KEY::A)) && KEY_NONE(KEY::W) && KEY_NONE(KEY::S))
	{
		m_Dir = eDIR::LEFT;
		ChangeState(L"BlockState");
	}
	if ((KEY_TAP(KEY::S) || KEY_PRESSED(KEY::S)) && KEY_NONE(KEY::A) && KEY_NONE(KEY::D))
	{
		m_Dir = eDIR::DOWN;
		ChangeState(L"BlockState");
	}
	if ((KEY_TAP(KEY::D) || KEY_PRESSED(KEY::D)) && KEY_NONE(KEY::W) && KEY_NONE(KEY::S))
	{
		m_Dir = eDIR::RIGHT;
		ChangeState(L"BlockState");
	}

	if (KEY_NONE(KEY::LSHIFT))
	{
		ChangeState(L"StandState");
	}
}

void CPlayerBlockState::Enter()
{
	m_Dir = GetFSM()->GetStateMachine()->GetOwner()->GetDir();
	m_Anim = GetFSM()->GetStateMachine()->Animator2D();

	switch (m_Dir)
	{
	case eDIR::UP:
		m_Anim->Play(L"Block_Up");
		break;
	case eDIR::DOWN:
		m_Anim->Play(L"Block_Down");
		break;
	case eDIR::LEFT:
		m_Anim->Play(L"Block_Left");
		break;
	case eDIR::RIGHT:
		m_Anim->Play(L"Block_Right");
		break;
	case eDIR::UPLEFT:
		m_Anim->Play(L"Block_Up");
		break;
	case eDIR::UPRIGHT:
		m_Anim->Play(L"Block_Up");
		break;
	case eDIR::DOWNLEFT:
		m_Anim->Play(L"Block_Down");
		break;
	case eDIR::DOWNRIGHT:
		m_Anim->Play(L"Block_Down");
		break;
	}
}

void CPlayerBlockState::Exit()
{
	if (KEY_PRESSED(KEY::W)) m_Dir = eDIR::UP;
	if (KEY_PRESSED(KEY::A)) m_Dir = eDIR::LEFT;
	if (KEY_PRESSED(KEY::S)) m_Dir = eDIR::DOWN;
	if (KEY_PRESSED(KEY::D)) m_Dir = eDIR::RIGHT;

	if (KEY_PRESSED(KEY::W) && KEY_PRESSED(KEY::A)) m_Dir = eDIR::UPLEFT;
	else if (KEY_PRESSED(KEY::D) && KEY_PRESSED(KEY::W)) m_Dir = eDIR::UPRIGHT;
	else if (KEY_PRESSED(KEY::A) && KEY_PRESSED(KEY::S)) m_Dir = eDIR::DOWNLEFT;
	else if (KEY_PRESSED(KEY::S) && KEY_PRESSED(KEY::D)) m_Dir = eDIR::DOWNRIGHT;

	GetFSM()->GetStateMachine()->GetOwner()->SetDir(m_Dir);
}


