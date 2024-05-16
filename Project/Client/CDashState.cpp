#include "pch.h"
#include "CDashState.h"

#include <Engine/components.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>

#include <Scripts/CChainSystemScript.h>
#include <Scripts/CEffectScript.h>

CDashState::CDashState()
	: m_Anim(nullptr)
	, m_ChainSystem(nullptr)
	, m_MoveMent(nullptr)

{
}

CDashState::~CDashState()
{
}

void CDashState::finaltick()
{
	// 애니메이션 종료되면 체인시스템 시작
	m_Anim->GetCurAnim()->IsFinish() ? m_ChainSystem->SetStart(true) : m_ChainSystem->SetStart(false);

	// 체인 시스템 시작 && 타이밍이 < 리커버리타임 && 대쉬버튼
	if (!KEY_NONE(KEY::RBTN) && m_ChainSystem->IsStart() && m_ChainSystem->IsRecovery())
	{
		ChangeState(L"DashState");
		CEffectScript* pEffect = GetFSM()->GetStateMachine()->GetOwner()->GetScript<CEffectScript>();
		pEffect->OnEffect(eEffectStatus::CHAIN_EFFECT);
	}

	if (KEY_TAP(KEY::LBTN) && m_ChainSystem->IsStart())
	{
		ChangeState(L"AttackState");
	}

	if (KEY_TAP(KEY::LBTN) && !m_ChainSystem->IsStart())
	{
		ChangeState(L"DashAttState");
	}

	if ( KEY_NONE(KEY::RBTN)&& KEY_NONE(KEY::LBTN) && !m_ChainSystem->IsRecovery())
	{
		ChangeState(L"StandState");
	}

}

void CDashState::Enter()
{
	m_ChainSystem = GetFSM()->GetStateMachine()->GetOwner()->GetScript<CChainSystemScript>();

	m_MoveMent = GetFSM()->GetStateMachine()->Movement();
	m_Anim = GetFSM()->GetStateMachine()->Animator2D();

	m_Dir = GetFSM()->GetStateMachine()->GetOwner()->GetDir();
	Vec3 vPos = GetFSM()->GetStateMachine()->Transform()->GetRelativePos();
	Vec2 vVelocity = GetFSM()->GetStateMachine()->Movement()->GetVelocity();

	GetFSM()->GetStateMachine()->GetOwner()->SetAfterImgAct(true);

	m_MoveMent->SetMaxSpeed(550.f);
	switch (m_Dir)
	{
	case eDIR::UP:
		m_Anim->Play(L"Dash_Up", false);
		vVelocity.y += 550.f;
		break;
	case eDIR::DOWN:
		m_Anim->Play(L"Dash_Down", false);
		vVelocity.y += -550.f;
		break;
	case eDIR::LEFT:
		m_Anim->Play(L"Dash_Left", false);
		vVelocity.x += -550.f;
		break;
	case eDIR::RIGHT:
		m_Anim->Play(L"Dash_Right", false);
		vVelocity.x += 550.f;
		break;
	case eDIR::UPLEFT:
		m_Anim->Play(L"Dash_Up", false);
		vVelocity.x += -550.f; vVelocity.y += 550.f;
		break;
	case eDIR::UPRIGHT:
		m_Anim->Play(L"Dash_UpRight", false);
		vVelocity.x += 550.f; vVelocity.y += 550.f;
		break;
	case eDIR::DOWNLEFT:
		m_Anim->Play(L"Dash_Down", false);
		vVelocity.x += -550.f; vVelocity.y += -550.f;
		break;
	case eDIR::DOWNRIGHT:
		m_Anim->Play(L"Dash_DownRight", false);
		vVelocity.x += 550.f; vVelocity.y += -550.f;
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
}

void CDashState::Exit()
{
	if ((KEY_TAP(KEY::W) || KEY_PRESSED(KEY::W)) && KEY_NONE(KEY::A) && KEY_NONE(KEY::S)) m_Dir = eDIR::UP;
	if ((KEY_TAP(KEY::A) || KEY_PRESSED(KEY::A)) && KEY_NONE(KEY::W) && KEY_NONE(KEY::S)) m_Dir = eDIR::LEFT;
	if ((KEY_TAP(KEY::S) || KEY_PRESSED(KEY::S)) && KEY_NONE(KEY::A) && KEY_NONE(KEY::D)) m_Dir = eDIR::DOWN;
	if ((KEY_TAP(KEY::D) || KEY_PRESSED(KEY::D)) && KEY_NONE(KEY::W) && KEY_NONE(KEY::S)) m_Dir = eDIR::RIGHT;

	if (KEY_PRESSED(KEY::W) && KEY_PRESSED(KEY::A)) m_Dir = eDIR::UPLEFT;
	else if (KEY_PRESSED(KEY::D) && KEY_PRESSED(KEY::W)) m_Dir = eDIR::UPRIGHT;
	else if (KEY_PRESSED(KEY::A) && KEY_PRESSED(KEY::S)) m_Dir = eDIR::DOWNLEFT;
	else if (KEY_PRESSED(KEY::S) && KEY_PRESSED(KEY::D)) m_Dir = eDIR::DOWNRIGHT;

	//m_MoveMent->SetMaxSpeed(125.f);
	
	m_ChainSystem->Clear();
	GetFSM()->GetStateMachine()->GetOwner()->SetDir(m_Dir);
}

