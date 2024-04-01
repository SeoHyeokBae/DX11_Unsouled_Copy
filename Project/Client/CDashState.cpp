#include "pch.h"
#include "CDashState.h"

#include <Engine/components.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>

#include <Scripts/CChainSystemScript.h>

CDashState::CDashState()
	: m_Anim(nullptr)
	, m_ChainSystem(nullptr)

{
}

CDashState::~CDashState()
{
}

void CDashState::finaltick()
{
	// �ִϸ��̼� ����Ǹ� ü�νý��� ����
	m_Anim->GetCurAnim()->IsFinish() ? m_ChainSystem->SetStart(true) : m_ChainSystem->SetStart(false);

	// ü�� �ý��� ���� && Ÿ�̹��� < ��Ŀ����Ÿ�� && �뽬��ư
	if (!KEY_NONE(KEY::RBTN) && m_ChainSystem->IsStart() && m_ChainSystem->IsRecovery())
	{
		ChangeState(L"DashState");
	}

	if (KEY_NONE(KEY::RBTN) && !m_ChainSystem->IsRecovery())
	{
		ChangeState(L"StandState");
	}
}

void CDashState::Enter()
{
	m_ChainSystem = GetFSM()->GetStateMachine()->GetOwner()->GetScript<CChainSystemScript>();

	m_Dir = GetFSM()->GetStateMachine()->GetOwner()->GetDir();
	m_Anim = GetFSM()->GetStateMachine()->Animator2D();
	Vec3 vPos = GetFSM()->GetStateMachine()->Transform()->GetRelativePos();
	Vec2 vVelocity = GetFSM()->GetStateMachine()->Movement()->GetVelocity();
	Vec2 vForce = Vec2(0.f, 0.f);

	GetFSM()->GetStateMachine()->GetOwner()->SetAfterImgAct(true);

	switch (m_Dir)
	{
	case eDIR::UP:
		m_Anim->Play(L"Dash_Up", false);
		vVelocity.y += 400.f;
		break;
	case eDIR::DOWN:
		m_Anim->Play(L"Dash_Down", false);
		vVelocity.y += -400.f;
		break;
	case eDIR::LEFT:
		m_Anim->Play(L"Dash_Left", false);
		vVelocity.x += -400.f;
		break;
	case eDIR::RIGHT:
		m_Anim->Play(L"Dash_Right", false);
		vVelocity.x += 400.f;
		break;
	case eDIR::UPLEFT:
		m_Anim->Play(L"Dash_Up", false);
		vVelocity.x += -400.f; vVelocity.y += 400.f;
		break;
	case eDIR::UPRIGHT:
		m_Anim->Play(L"Dash_UpRight", false);
		vVelocity.x += 400.f; vVelocity.y += 400.f;
		break;
	case eDIR::DOWNLEFT:
		m_Anim->Play(L"Dash_Down", false);
		vVelocity.x += -400.f; vVelocity.y += -400.f;
		break;
	case eDIR::DOWNRIGHT:
		m_Anim->Play(L"Dash_DownRight", false);
		vVelocity.x += 400.f; vVelocity.y += -400.f;
		break;
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

	m_ChainSystem->Clear();
	GetFSM()->GetStateMachine()->GetOwner()->SetDir(m_Dir);
}

