#include "pch.h"
#include "CNiug_BallistaSoulderState.h"

#include <Engine/components.h>
#include <Engine/CTimeMgr.h>

CNiug_BallistaSoulderState::CNiug_BallistaSoulderState()
{
}

CNiug_BallistaSoulderState::~CNiug_BallistaSoulderState()
{
}
void CNiug_BallistaSoulderState::finaltick()
{
	CAnimator2D* anim = GetFSM()->GetStateMachine()->Animator2D();

	if (anim->GetCurAnim()->IsFinish())
		ChangeState(L"RunningState");
}

void CNiug_BallistaSoulderState::Enter()
{
	eDIR dir = GetFSM()->GetStateMachine()->GetOwner()->GetDir();
	CAnimator2D* anim = GetFSM()->GetStateMachine()->Animator2D();

	CGameObject* pTarget = ((CGameObject*)GetBlackboardData(L"TargetObject"));
	CGameObject* pSelf = GetFSM()->GetStateMachine()->GetOwner();
	Vec3 vDir = pTarget->Transform()->GetWorldPos() - pSelf->Transform()->GetWorldPos();
	vDir.Normalize();

	GetFSM()->GetStateMachine()->GetOwner()->SetAfterImgAct(true);

	Vec3 vPos = vDir * 750.f;
	if (eDIR::RIGHT == dir)
	{
		anim->Play(L"Niug_BallistaSoulder_Right",false);
	}
	else
	{
		anim->Play(L"Niug_BallistaSoulder_Left",false);
	}
	pSelf->Movement()->SetVelocity(Vec2(vPos.x, vPos.y));

}

void CNiug_BallistaSoulderState::Exit()
{
	GetFSM()->GetStateMachine()->GetOwner()->SetAfterImgAct(false);
}


