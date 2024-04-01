#include "pch.h"
#include "CAttackState.h"

#include <Engine/components.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>

#include <Scripts/CChainSystemScript.h>

CAttackState::CAttackState()
	: m_Anim(nullptr)
	, m_ChainSystem(nullptr)
	, m_Combo(0)
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
	m_Anim->GetCurAnim()->IsFinish() ? m_ChainSystem->SetStart(true) : m_ChainSystem->SetStart(false);

	// 체인 시스템 시작 && 타이밍이 < 리커버리타임 && 공격버튼
	if (!KEY_NONE(KEY::LBTN) && m_ChainSystem->IsStart() && m_ChainSystem->IsRecovery())
	{
		ChangeState(L"AttackState");
	}

	if (KEY_NONE(KEY::LBTN) && !m_ChainSystem->IsRecovery())
	{
		m_Combo = 0;
		ChangeState(L"StandState");
	}

	GetFSM()->GetStateMachine()->Transform()->SetRelativePos(vPos);
	GetFSM()->GetStateMachine()->Movement()->SetVelocity(vVelocity);

}

void CAttackState::Enter()
{
	m_ChainSystem = GetFSM()->GetStateMachine()->GetOwner()->GetScript<CChainSystemScript>();

	if (m_Combo > 3) m_Combo = 0;

	m_Combo++;
	m_Dir = GetFSM()->GetStateMachine()->GetOwner()->GetDir();
	m_Anim = GetFSM()->GetStateMachine()->Animator2D();
	Vec2 vForce = Vec2(0.f,0.f);

	if (KEY_PRESSED(KEY::W) && KEY_PRESSED(KEY::A)) m_Dir = eDIR::UPLEFT;
	else if (KEY_PRESSED(KEY::D) && KEY_PRESSED(KEY::W)) m_Dir = eDIR::UPRIGHT;
	else if (KEY_PRESSED(KEY::A) && KEY_PRESSED(KEY::S)) m_Dir = eDIR::DOWNLEFT;
	else if (KEY_PRESSED(KEY::S) && KEY_PRESSED(KEY::D)) m_Dir = eDIR::DOWNRIGHT;

	GetFSM()->GetStateMachine()->GetOwner()->SetAfterImgAct(true);

	switch (m_Dir)
	{
	case eDIR::UP:
		if (1 == m_Combo) m_Anim->Play(L"BAttack01_Up", false);
		if (2 == m_Combo) m_Anim->Play(L"BAttack02_Up", false);
		if (3 == m_Combo) m_Anim->Play(L"BAttack03_Up", false);
		vForce.y = 900.f;
		break;
	case eDIR::DOWN:
		if (1 == m_Combo) m_Anim->Play(L"BAttack01_Down", false);
		if (2 == m_Combo) m_Anim->Play(L"BAttack02_Down", false);
		if (3 == m_Combo) m_Anim->Play(L"BAttack03_Down", false);
		vForce.y = -900.f;
		break;
	case eDIR::LEFT:
		if (1 == m_Combo) m_Anim->Play(L"BAttack01_Left", false);
		if (2 == m_Combo) m_Anim->Play(L"BAttack02_Left", false);
		if (3 == m_Combo) m_Anim->Play(L"BAttack03_Left", false);
		vForce.x = -900.f;
		break;
	case eDIR::RIGHT:
		if (1 == m_Combo) m_Anim->Play(L"BAttack01_Right", false);
		if (2 == m_Combo) m_Anim->Play(L"BAttack02_Right", false);
		if (3 == m_Combo) m_Anim->Play(L"BAttack03_Right", false);
		vForce.x = 900.f;
		break;
	case eDIR::UPLEFT:
		if (1 == m_Combo) m_Anim->Play(L"BAttack01_Left", false);
		if (2 == m_Combo) m_Anim->Play(L"BAttack02_Left", false);
		if (3 == m_Combo) m_Anim->Play(L"BAttack03_Left", false);
		vForce.x = -900.f; vForce.y = 900.f;
		break;
	case eDIR::UPRIGHT:
		if (1 == m_Combo) m_Anim->Play(L"BAttack01_Up", false);
		if (2 == m_Combo) m_Anim->Play(L"BAttack02_Up", false);
		if (3 == m_Combo) m_Anim->Play(L"BAttack03_Up", false);
		vForce.x = 900.f; vForce.y = 900.f;
		break;
	case eDIR::DOWNLEFT:
		if (1 == m_Combo) m_Anim->Play(L"BAttack01_Down", false);
		if (2 == m_Combo) m_Anim->Play(L"BAttack02_Down", false);
		if (3 == m_Combo) m_Anim->Play(L"BAttack03_Down", false);
		vForce.x = -900.f; vForce.y = -900.f;
		break;
	case eDIR::DOWNRIGHT:
		if (1 == m_Combo) m_Anim->Play(L"BAttack01_Right", false);
		if (2 == m_Combo) m_Anim->Play(L"BAttack02_Right", false);
		if (3 == m_Combo) m_Anim->Play(L"BAttack03_Right", false);
		vForce.x = 900.f; vForce.y = -900.f;
		break;
	}

	GetFSM()->GetStateMachine()->Movement()->AddForce(vForce);
}

void CAttackState::Exit()
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

