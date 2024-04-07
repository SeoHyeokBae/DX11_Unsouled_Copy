#include "pch.h"
#include "CNiug_RageOfIsno.h"

#include <Engine/components.h>
#include <Engine/CTimeMgr.h>

#include <Scripts/CBossNiugScript.h>

CNiug_RageOfIsno::CNiug_RageOfIsno()
	: m_FrmCount(0)
	, m_LeftOrRight(0)
	, m_bOn(true)
{

}

CNiug_RageOfIsno::~CNiug_RageOfIsno()
{
}

void CNiug_RageOfIsno::finaltick()
{
	CAnimator2D* anim = GetFSM()->GetStateMachine()->Animator2D();
	CGameObject* pTarget = ((CGameObject*)GetBlackboardData(L"TargetObject"));
	CGameObject* pSelf = GetFSM()->GetStateMachine()->GetOwner();

	Vec3 vDir = pTarget->Transform()->GetWorldPos() - pSelf->Transform()->GetWorldPos();
	float fDis = vDir.Length();

	bool attacking = pSelf->GetScript<CBossNiugScript>()->IsAttacking();
	if (m_bOn && anim->GetCurAnim()->GetCurFrmIdx() == 7 )
	{
		// Hit Collider »ý¼º
		m_bOn = false;
		pSelf->GetScript<CBossNiugScript>()->OnAttacking();
	}

	if (anim->GetCurAnim()->IsFinish())
	{
		ChangeState(L"RunningState");
	}
}

void CNiug_RageOfIsno::Enter()
{
	eDIR dir = GetFSM()->GetStateMachine()->GetOwner()->GetDir();
	CAnimator2D* anim = GetFSM()->GetStateMachine()->Animator2D();

	if (m_LeftOrRight >= 6)
	{
		m_LeftOrRight = 0;
	}

	m_LeftOrRight++;

	if (m_LeftOrRight <= 3)
	{
		anim->Play(L"Niug_RageOfIsno_Left", false);
	}
	else
	{
		anim->Play(L"Niug_RageOfIsno_Right", false);
	}

}

void CNiug_RageOfIsno::Exit()
{
	m_bOn = true;
}

