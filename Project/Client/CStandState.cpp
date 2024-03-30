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
	m_Dir = *((eDIR*)GetBlackboardData(L"Dir"));
	CAnimator2D* anim = GetFSM()->GetStateMachine()->Animator2D();

	switch (m_Dir)
	{
	case eDIR::UP:
		anim->Play(L"Stand_Up");
		break;
	case eDIR::DOWN:
		anim->Play(L"Stand_Down");
		break;
	case eDIR::LEFT:
		anim->Play(L"Stand_Left");
		break;
	case eDIR::RIGHT:
		anim->Play(L"Stand_Right");
		break;
	case eDIR::NONE:
		break;
	default:
		break;
	}
}

void CStandState::Exit()
{
	//m_Dir = eDIR::NONE;

	if (!KEY_NONE(KEY::D))
	{
		m_Dir = eDIR::RIGHT;
	}
	if (!KEY_NONE(KEY::S))
	{
		m_Dir = eDIR::DOWN;
	}
	if (!KEY_NONE(KEY::A))
	{
		m_Dir = eDIR::LEFT;
	}
	if (!KEY_NONE(KEY::W))
	{
		m_Dir = eDIR::UP;
	}
	
	SetBlackboardData(L"Dir", &m_Dir);

}
