#include "pch.h"
#include "CAttackState.h"

#include <Engine/components.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>

CAttackState::CAttackState()
	: m_Anim(nullptr)
{
}

CAttackState::~CAttackState()
{
}

void CAttackState::finaltick()
{


	if (KEY_NONE(KEY::LBTN) && m_Anim->GetCurAnim()->IsFinish())
	{
		ChangeState(L"StandState");
	}
}

void CAttackState::Enter()
{
	m_Dir = *((eDIR*)GetBlackboardData(L"Dir"));
	m_Anim = GetFSM()->GetStateMachine()->Animator2D();

	switch (m_Dir)
	{
	case eDIR::UP:
		m_Anim->Play(L"BAttack01_Up", false);
		break;
	case eDIR::DOWN:
		m_Anim->Play(L"BAttack01_Down", false);
		break;
	case eDIR::LEFT:
		m_Anim->Play(L"BAttack01_Left", false);
		break;
	case eDIR::RIGHT:
		m_Anim->Play(L"BAttack01_Right", false);
		break;
	case eDIR::NONE:
		break;
	default:
		break;
	}
}

void CAttackState::Exit()
{
	SetBlackboardData(L"Dir", &m_Dir);
}

