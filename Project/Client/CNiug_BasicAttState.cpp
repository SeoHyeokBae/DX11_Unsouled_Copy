#include "pch.h"
#include "CNiug_BasicAttState.h"

#include <Engine/components.h>
#include <Engine/CTimeMgr.h>
#include <Scripts/CBossNiugAttColScript.h>
#include <Scripts/CBossNiugScript.h>

CNiug_BasicAttState::CNiug_BasicAttState()
	: m_Combo(0)
{
}

CNiug_BasicAttState::~CNiug_BasicAttState()
{
}

void CNiug_BasicAttState::finaltick()
{
	CAnimator2D* anim = GetFSM()->GetStateMachine()->Animator2D();

	if (anim->GetCurAnim()->IsFinish())
		ChangeState(L"RunningState");

}

void CNiug_BasicAttState::Enter()
{
	if (m_Combo > 3) m_Combo = 0;

	eDIR dir = GetFSM()->GetStateMachine()->GetOwner()->GetDir();
	CAnimator2D* anim = GetFSM()->GetStateMachine()->Animator2D();

	CGameObject* pTarget = ((CGameObject*)GetBlackboardData(L"TargetObject"));
	CGameObject* pSelf = GetFSM()->GetStateMachine()->GetOwner();
	Vec3 vDir = pTarget->Transform()->GetWorldPos() - pSelf->Transform()->GetWorldPos();
	vDir.Normalize();

	m_Combo++;

	Vec3 vPos = vDir * 250.f;
	if (eDIR::RIGHT == dir)
	{
		if (1 == m_Combo) 
			anim->Play(L"Niug_BasicAtt01_Right", false);
		if (2 == m_Combo) 
			anim->Play(L"Niug_BasicAtt02_Right", false);
		if (3 == m_Combo)
		{
			anim->Play(L"Niug_BasicAtt03_Right", false);
			vPos = vDir * 350.f;
		}
	}
	else
	{
		if (1 == m_Combo) 
			anim->Play(L"Niug_BasicAtt01_Left", false);
		if (2 == m_Combo) 
			anim->Play(L"Niug_BasicAtt02_Left", false);
		if (3 == m_Combo)
		{
			anim->Play(L"Niug_BasicAtt03_Left", false);
			vPos = vDir * 350.f;
		}
	}
	pSelf->Movement()->SetVelocity(Vec2(vPos.x, vPos.y));

	
	pSelf->GetScript<CBossNiugScript>()->GetAttCol()->GetScript<CBossNiugAttColScript>()->On();
}

void CNiug_BasicAttState::Exit()
{
	
}


