#include "pch.h"
#include "CTraceState.h"


#include <Engine/CGameObject.h>
#include <Engine/CStateMachine.h>
#include <Engine/CTransform.h>
#include <Engine/CMovement.h>
#include <Engine/CTimeMgr.h>

CTraceState::CTraceState()
{
}

CTraceState::~CTraceState()
{
}


void CTraceState::finaltick()
{
	// 플레이어가 근처에 있으면, Trace 상태로 변경한다.	
	float Speed = *((float*)GetBlackboardData(L"Speed"));
	CGameObject* pTarget = ((CGameObject*)GetBlackboardData(L"TargetObject"));
	CGameObject* pSelf = GetFSM()->GetStateMachine()->GetOwner();

	Vec3 vDir = pTarget->Transform()->GetWorldPos() - pSelf->Transform()->GetWorldPos();
	vDir.Normalize();

	//Vec3 vPos = pSelf->Transform()->GetWorldPos() + vDir * DT * Speed;
	//Vec3 vPos = vDir * DT * Speed;
	//pSelf->Transform()->SetRelativePos(vPos);
	Vec3 vPos = vDir * Speed;
	//pSelf->Movement()->SetVelocity(vPos.XY());
}

void CTraceState::Enter()
{
}

void CTraceState::Exit()
{
}