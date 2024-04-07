#include "pch.h"
#include "CNiug_RunningState.h"

#include <Engine/components.h>
#include <Engine/CTimeMgr.h>

CNiug_RunningState::CNiug_RunningState()
	:m_FrmCount(0)
	, m_PrvDir(eDIR::NONE)
{
}

CNiug_RunningState::~CNiug_RunningState()
{
}

void CNiug_RunningState::finaltick()
{
	// �÷��̾ ��ó�� ������, Trace ���·� �����Ѵ�.	
	float Speed = *((float*)GetBlackboardData(L"Speed"));
	CGameObject* pTarget = ((CGameObject*)GetBlackboardData(L"TargetObject"));
	CGameObject* pSelf = GetFSM()->GetStateMachine()->GetOwner();

	
	Vec3 vDir = pTarget->Transform()->GetWorldPos() - pSelf->Transform()->GetWorldPos();
	float fDis = vDir.Length();

	if (300.f < fDis)
	{
		ChangeState(L"RageOfIsnoState");
		return;
	}

	if (65.f >= fDis && GetFSM()->GetCurStateName() == L"RunningState")
	{
		// �������� ������ ���
		ChangeState(L"BasicAttState");
	}

	else // ����
	{
		Vec3 vMove = -vDir;
		vDir.Normalize();

		if (fDis > 10.f)
		{
			Vec3 vPos = vDir * Speed;
			pSelf->Movement()->SetVelocity(vPos.XY());
		}

		// ������ȯ
		if (vDir.x > 0)
			pSelf->SetDir(eDIR::RIGHT);
		else
			pSelf->SetDir(eDIR::LEFT);

		if (m_PrvDir != pSelf->GetDir())
		{
			m_FrmCount = pSelf->Animator2D()->GetCurAnim()->GetCurFrmIdx();
			ChangeState(L"RunningState");
		}
	}



	// Ư�� �Ÿ����� �ٷ� ���� ���� changestate 


}

void CNiug_RunningState::Enter()
{
	eDIR dir = GetFSM()->GetStateMachine()->GetOwner()->GetDir();
	CAnimator2D* anim = GetFSM()->GetStateMachine()->Animator2D();
	m_PrvDir = dir;

	if (m_FrmCount != 0)
	{
		anim->GetCurAnim()->SetCurFrmIdx(m_FrmCount);
		m_FrmCount = 0;
	}

	if (eDIR::RIGHT == dir)
	{
		anim->Play(L"Niug_Running_Right");
	}
	else
	{
		anim->Play(L"Niug_Running_Left");
	}
}

void CNiug_RunningState::Exit()
{
	m_PrvDir = eDIR::NONE;
}

