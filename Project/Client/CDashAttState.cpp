#include "pch.h"
#include "CDashAttState.h"

#include <Engine/components.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>

#include <Scripts/CChainSystemScript.h>
#include <Scripts/CPlayerScript.h>

CDashAttState::CDashAttState()
	: m_Anim(nullptr)
	, m_ChainSystem(nullptr)
{
}

CDashAttState::~CDashAttState()
{
}

void CDashAttState::finaltick()
{
	// 애니메이션 종료되면 체인시스템 시작
	m_Anim->GetCurAnim()->IsFinish() ? m_ChainSystem->SetStart(true) : m_ChainSystem->SetStart(false);

	//// 체인 시스템 시작 && 타이밍이 < 리커버리타임 && 대쉬버튼
	//if (KEY_TAP(KEY::LBTN) && m_ChainSystem->IsStart() && m_ChainSystem->IsRecovery())
	//{
	//	ChangeState(L"AttackState");
	//}

	if (KEY_NONE(KEY::RBTN)&& KEY_NONE(KEY::LBTN) && !m_ChainSystem->IsRecovery())
	{
		ChangeState(L"StandState");
	}
}

void CDashAttState::Enter()
{
	m_ChainSystem = GetFSM()->GetStateMachine()->GetOwner()->GetScript<CChainSystemScript>();

	m_Dir = GetFSM()->GetStateMachine()->GetOwner()->GetDir();
	m_Anim = GetFSM()->GetStateMachine()->Animator2D();

	GetFSM()->GetStateMachine()->GetOwner()->SetAfterImgAct(true);
	Vec2 vVelocity = GetFSM()->GetStateMachine()->Movement()->GetVelocity();

	switch (m_Dir)
	{
	case eDIR::UP:
		m_Anim->Play(L"DashAtt_Up", false);
		break;
	case eDIR::DOWN:
		m_Anim->Play(L"DashAtt_Down", false);
		break;
	case eDIR::LEFT:
		m_Anim->Play(L"DashAtt_Left", false);
		break;
	case eDIR::RIGHT:
		m_Anim->Play(L"DashAtt_Right", false);
		break;
	case eDIR::UPLEFT:
		m_Anim->Play(L"DashAtt_UpLeft", false);
		break;
	case eDIR::UPRIGHT:
		m_Anim->Play(L"DashAtt_UpRight", false);
		break;
	case eDIR::DOWNLEFT:
		m_Anim->Play(L"DashAtt_DownLeft", false);
		break;
	case eDIR::DOWNRIGHT:
		m_Anim->Play(L"DashAtt_DownRight", false);
		break;
	}

	// 대각선 거리 보정
	if (vVelocity.x != 0 && vVelocity.y != 0)
	{
		Vec2 newVelo = vVelocity;
		newVelo /= vVelocity.Length();
		vVelocity.x *= fabs(newVelo.x);
		vVelocity.y *= fabs(newVelo.y);
	}

	GetFSM()->GetStateMachine()->Movement()->SetVelocity(vVelocity);

	GetFSM()->GetStateMachine()->GetOwner()->GetScript<CPlayerScript>()->OnHit();
}

void CDashAttState::Exit()
{
	if ((KEY_TAP(KEY::W) || KEY_PRESSED(KEY::W)) && KEY_NONE(KEY::A) && KEY_NONE(KEY::S)) m_Dir = eDIR::UP;
	if ((KEY_TAP(KEY::A) || KEY_PRESSED(KEY::A)) && KEY_NONE(KEY::W) && KEY_NONE(KEY::S)) m_Dir = eDIR::LEFT;
	if ((KEY_TAP(KEY::S) || KEY_PRESSED(KEY::S)) && KEY_NONE(KEY::A) && KEY_NONE(KEY::D)) m_Dir = eDIR::DOWN;
	if ((KEY_TAP(KEY::D) || KEY_PRESSED(KEY::D)) && KEY_NONE(KEY::W) && KEY_NONE(KEY::S)) m_Dir = eDIR::RIGHT;

	if (KEY_PRESSED(KEY::W) && KEY_PRESSED(KEY::A)) m_Dir = eDIR::UPLEFT;
	else if (KEY_PRESSED(KEY::D) && KEY_PRESSED(KEY::W)) m_Dir = eDIR::UPRIGHT;
	else if (KEY_PRESSED(KEY::A) && KEY_PRESSED(KEY::S)) m_Dir = eDIR::DOWNLEFT;
	else if (KEY_PRESSED(KEY::S) && KEY_PRESSED(KEY::D)) m_Dir = eDIR::DOWNRIGHT;

	m_ChainSystem->Clear();
	GetFSM()->GetStateMachine()->GetOwner()->SetDir(m_Dir);
}


