#include "pch.h"
#include "CNiug_BallistaReadyState.h"

#include <Engine/components.h>
#include <Engine/CTimeMgr.h>

CNiug_BallistaReadyState::CNiug_BallistaReadyState()
	: m_FrmCount(0)
	, m_fAcc(0.f)
{
}

CNiug_BallistaReadyState::~CNiug_BallistaReadyState()
{
}

void CNiug_BallistaReadyState::finaltick()
{
	m_fAcc += DT;
	if (m_fAcc > 1.0f)
	{
		ChangeState(L"BallistaSoulderState");
		return;
	}

}

void CNiug_BallistaReadyState::Enter()
{
	eDIR dir = GetFSM()->GetStateMachine()->GetOwner()->GetDir();
	CAnimator2D* anim = GetFSM()->GetStateMachine()->Animator2D();

	CGameObject* pTarget = ((CGameObject*)GetBlackboardData(L"TargetObject"));
	CGameObject* pSelf = GetFSM()->GetStateMachine()->GetOwner();


	if (eDIR::RIGHT == dir)
	{
		anim->Play(L"Niug_BllistaReady_Right");
	}
	else
	{
		anim->Play(L"Niug_BllistaReady_Left");
	}
}

void CNiug_BallistaReadyState::Exit()
{
	m_FrmCount = 0;
	m_fAcc = 0.f;
}

