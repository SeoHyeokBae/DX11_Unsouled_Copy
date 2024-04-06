#include "pch.h"
#include "CIdleState.h"

#include <Engine/components.h>
#include <Engine/CGameObject.h>
#include <Engine/CStateMachine.h>
#include <Engine/CTimeMgr.h>

CIdleState::CIdleState()
{
}

CIdleState::~CIdleState()
{
}

void CIdleState::finaltick()
{
	// �÷��̾ ��ó�� ������, Trace ���·� �����Ѵ�.
	float DetectRange = *((float*)GetBlackboardData(L"DetectRange"));
	CGameObject* pTarget = ((CGameObject*)GetBlackboardData(L"TargetObject"));

	CGameObject* pSelf = GetFSM()->GetStateMachine()->GetOwner();

	Vec3 vDist = pTarget->Transform()->GetWorldPos() - pSelf->Transform()->GetWorldPos();

	GamePlayStatic::DrawDebugCircle(pSelf->Transform()->GetWorldPos(), DetectRange, Vec3(0.1f, 1.f, 0.1f), 0.f);

	if (vDist.Length() <= DetectRange)
	{
		// �������·� �����Ѵ�.
		ChangeState(L"TraceState");
	}
}

void CIdleState::Enter()
{
	m_Dir = GetFSM()->GetStateMachine()->GetOwner()->GetDir();
	CAnimator2D* anim = GetFSM()->GetStateMachine()->Animator2D();

	// ���߿� ���� �ִϸ��̼� �̸� ����
	switch (m_Dir)
	{
	case eDIR::UP:
		anim->Play(L"Stand_Right", true);
		break;
	case eDIR::DOWN:
		anim->Play(L"Stand_Left", true);
		break;
	case eDIR::LEFT:
		anim->Play(L"Stand_Left", true);
		break;
	case eDIR::RIGHT:
		anim->Play(L"Stand_Right", true);
		break;
	case eDIR::UPLEFT:
		anim->Play(L"Stand_Left", true);
		break;
	case eDIR::UPRIGHT:
		anim->Play(L"Stand_Right", true);
		break;
	case eDIR::DOWNLEFT:
		anim->Play(L"Stand_Left", true);
		break;
	case eDIR::DOWNRIGHT:
		anim->Play(L"Stand_Right", true);
		break;
	}
}

void CIdleState::Exit()
{

}