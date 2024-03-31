#include "pch.h"
#include "CAttackState.h"

#include <Engine/components.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>

// 애니메이션 끝나고 -> 
// 채인 타이밍 / 리커버리 타임 설정
// + Anim 종료후 시간 지정 0.? 초 단위
// 
CAttackState::CAttackState()
	: m_Anim(nullptr)
	, m_Combo(0)
	, m_fTiming(0.f)
	, m_bChain(false)
	, m_bStart(false)
{
}

CAttackState::~CAttackState()
{
}

void CAttackState::finaltick()
{
	float Speed = 20.f;
	Vec3 vPos = GetFSM()->GetStateMachine()->Transform()->GetRelativePos();
	Vec2 vVelocity = GetFSM()->GetStateMachine()->Movement()->GetVelocity();

	// 애니메이션 종료되면 체인시스템 시작
	m_Anim->GetCurAnim()->IsFinish() ? m_bStart = true : m_bStart = false;
	SetBlackboardData(L"Chain", &m_bStart);
	
	// 타이밍 시간 증가
	if (m_bStart)
	{
		m_fTiming += DT;
	}

	// 체인 타이밍
	if (m_fTiming >= 0.02f && m_fTiming <= 0.07f)
		m_bChain = true;

	// 체인 시스템 시작 && 타이밍이 < 리커버리타임 && 공격버튼
	if (m_bStart && RECOVERYTIME > m_fTiming && !KEY_NONE(KEY::LBTN))
	{
		vVelocity.x = -450.f;
		ChangeState(L"AttackState");
	}

	if (RECOVERYTIME <= m_fTiming && KEY_NONE(KEY::LBTN))
	{
		m_Combo = 0;
		m_fTiming = 0.f;
		ChangeState(L"StandState");
	}



	if (KEY_PRESSED(KEY::W))
	{
	}
	if (KEY_TAP(KEY::W))
	{
	}
	if (KEY_RELEASED(KEY::W))
	{
	}


	if (KEY_PRESSED(KEY::A))
	{
	}
	if (KEY_TAP(KEY::A) && KEY_NONE(KEY::D))
	{
	}
	if (KEY_RELEASED(KEY::A))
	{
	}

	if (KEY_PRESSED(KEY::S))
	{
	}
	if (KEY_TAP(KEY::S))
	{
	}
	if (KEY_RELEASED(KEY::S))
	{
	}

	if (KEY_PRESSED(KEY::D))
	{
		//vPos.x += DT * 200.f;
		//vVelocity.x = 300.f;
	}
	if (KEY_TAP(KEY::D) && KEY_NONE(KEY::A))
	{
	}
	if (KEY_RELEASED(KEY::D))
	{
	}

	GetFSM()->GetStateMachine()->Transform()->SetRelativePos(vPos);
	GetFSM()->GetStateMachine()->Movement()->SetVelocity(vVelocity);

}

void CAttackState::Enter()
{
	if (m_Combo > 3) m_Combo = 0;

	m_Combo++;
	m_fTiming = 0.f;
	m_Dir = GetFSM()->GetStateMachine()->GetOwner()->GetDir();
	m_Anim = GetFSM()->GetStateMachine()->Animator2D();
	Vec2 vVelocity = GetFSM()->GetStateMachine()->Movement()->GetVelocity();

	if (KEY_PRESSED(KEY::W) && KEY_PRESSED(KEY::A)) m_Dir = eDIR::LEFT;
	if (KEY_PRESSED(KEY::A) && KEY_PRESSED(KEY::S)) m_Dir = eDIR::DOWN;
	if (KEY_PRESSED(KEY::S) && KEY_PRESSED(KEY::D)) m_Dir = eDIR::RIGHT;
	if (KEY_PRESSED(KEY::D) && KEY_PRESSED(KEY::W)) m_Dir = eDIR::UP;

	GetFSM()->GetStateMachine()->GetOwner()->SetAfterImgAct(true);

	if (1 == m_Combo)
	{
		switch (m_Dir)
		{
		case eDIR::UP:
			m_Anim->Play(L"BAttack01_Up", false);
			break;
		case eDIR::DOWN:
			m_Anim->Play(L"BAttack01_Down", false);
			break;
		case eDIR::LEFT:
			m_Anim->Play(L"BAttack01_Left", false);
			break;
		case eDIR::RIGHT:
			m_Anim->Play(L"BAttack01_Right", false);
			break;
		case eDIR::NONE:
			break;
		default:
			break;
		}
	}
	else if (2 == m_Combo)
	{
		switch (m_Dir)
		{
		case eDIR::UP:
			m_Anim->Play(L"BAttack02_Up", false);
			break;
		case eDIR::DOWN:
			m_Anim->Play(L"BAttack02_Down", false);
			break;
		case eDIR::LEFT:
			m_Anim->Play(L"BAttack02_Left", false);
			break;
		case eDIR::RIGHT:
			m_Anim->Play(L"BAttack02_Right", false);
			break;
		case eDIR::NONE:
			break;
		default:
			break;
		}
	}
	else if (3 == m_Combo)
	{
		switch (m_Dir)
		{
		case eDIR::UP:
			m_Anim->Play(L"BAttack03_Up", false);
			break;
		case eDIR::DOWN:
			m_Anim->Play(L"BAttack03_Down", false);
			break;
		case eDIR::LEFT:
			m_Anim->Play(L"BAttack03_Left", false);
			break;
		case eDIR::RIGHT:
			m_Anim->Play(L"BAttack03_Right", false);
			break;
		case eDIR::NONE:
			break;
		default:
			break;
		}
	}
	vVelocity.x = -450.f;
	GetFSM()->GetStateMachine()->Movement()->SetVelocity(vVelocity);
}

void CAttackState::Exit()
{
	m_bStart = false;
	SetBlackboardData(L"Chain", &m_bStart);
	GetFSM()->GetStateMachine()->GetOwner()->SetDir(m_Dir);
}

