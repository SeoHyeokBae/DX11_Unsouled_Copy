#include "pch.h"
#include "CAttackState.h"

#include <Engine/components.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>

// �ִϸ��̼� ������ -> 
// ä�� Ÿ�̹� / ��Ŀ���� Ÿ�� ����
// + Anim ������ �ð� ���� 0.? �� ����

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
	// �ִϸ��̼� ����Ǹ� ü�νý��� ����
	m_Anim->GetCurAnim()->IsFinish() ? m_bStart = true : m_bStart = false;
	
	// Ÿ�̹� �ð� ����
	if(m_bStart)
		m_fTiming += DT;

	// ü�� Ÿ�̹�
	if (m_fTiming >= 0.02f && m_fTiming <= 0.07f)
		m_bChain = true;

	// ü�� �ý��� ���� && Ÿ�̹��� < ��Ŀ����Ÿ�� && ���ݹ�ư
	if (m_bStart && RECOVERYTIME > m_fTiming && !KEY_NONE(KEY::LBTN))
	{
		ChangeState(L"AttackState");
	}


	if (RECOVERYTIME <= m_fTiming && KEY_NONE(KEY::LBTN))
	{
		m_Combo = 0;
		m_fTiming = 0.f;
		ChangeState(L"StandState");
	}
}

void CAttackState::Enter()
{
	if (m_Combo > 3) m_Combo = 0;

	m_Combo++;
	m_fTiming = 0.f;
	m_Dir = *((eDIR*)GetBlackboardData(L"Dir"));
	m_Anim = GetFSM()->GetStateMachine()->Animator2D();

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


}

void CAttackState::Exit()
{
	m_bStart = false;
	SetBlackboardData(L"Dir", &m_Dir);
}

