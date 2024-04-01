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
	m_Dir = GetFSM()->GetStateMachine()->GetOwner()->GetDir();
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
	case eDIR::UPLEFT:
		anim->Play(L"Stand_Up");
		break;
	case eDIR::UPRIGHT:
		anim->Play(L"Stand_Up");
		break;
	case eDIR::DOWNLEFT:
		anim->Play(L"Stand_Down");
		break;
	case eDIR::DOWNRIGHT:
		anim->Play(L"Stand_Down");
		break;
	}
}

void CStandState::Exit()
{
	if ((KEY_TAP(KEY::W) || KEY_PRESSED(KEY::W)) && KEY_NONE(KEY::A) && KEY_NONE(KEY::S)) m_Dir = eDIR::UP;
	if ((KEY_TAP(KEY::A) || KEY_PRESSED(KEY::A)) && KEY_NONE(KEY::W) && KEY_NONE(KEY::S)) m_Dir = eDIR::LEFT;
	if ((KEY_TAP(KEY::S) || KEY_PRESSED(KEY::S)) && KEY_NONE(KEY::A) && KEY_NONE(KEY::D)) m_Dir = eDIR::DOWN;
	if ((KEY_TAP(KEY::D) || KEY_PRESSED(KEY::D)) && KEY_NONE(KEY::W) && KEY_NONE(KEY::S)) m_Dir = eDIR::RIGHT;

	if (KEY_PRESSED(KEY::W) && KEY_PRESSED(KEY::A)) m_Dir = eDIR::UPLEFT;
	else if (KEY_PRESSED(KEY::D) && KEY_PRESSED(KEY::W)) m_Dir = eDIR::UPRIGHT;
	else if (KEY_PRESSED(KEY::A) && KEY_PRESSED(KEY::S)) m_Dir = eDIR::DOWNLEFT;
	else if (KEY_PRESSED(KEY::S) && KEY_PRESSED(KEY::D)) m_Dir = eDIR::DOWNRIGHT;

	
	GetFSM()->GetStateMachine()->GetOwner()->SetDir(m_Dir);
}
