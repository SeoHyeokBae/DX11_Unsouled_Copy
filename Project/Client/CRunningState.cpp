#include "pch.h"
#include "CRunningState.h"

#include <Engine/components.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>

CRunningState::CRunningState()
	:m_Anim(nullptr)
{
}

CRunningState::~CRunningState()
{
}

void CRunningState::finaltick()
{
	float Speed = *((float*)GetBlackboardData(L"Speed"));
	Vec3 vPos = GetFSM()->GetStateMachine()->Transform()->GetRelativePos();
	Vec3 vCalPos = Vec3(0.f,0.f,0.f);

	if (KEY_PRESSED(KEY::W))
	{
		vCalPos.y += DT * Speed;
	}
	if (KEY_PRESSED(KEY::A))
	{
		vCalPos.x -= DT * Speed;
	}
	if (KEY_PRESSED(KEY::S))
	{
		vCalPos.y-= DT * Speed;
	}
	if (KEY_PRESSED(KEY::D))
	{
		vCalPos.x += DT * Speed;
	}

	if (vCalPos.x > 0 && vCalPos.y >0)
	{
		m_Dir = eDIR::UPRIGHT;
	}
	else if (vCalPos.x == 0 && vCalPos.y > 0)
	{
		m_Dir = eDIR::UP;
	}
	else if (vCalPos.x < 0 && vCalPos.y > 0)
	{
		m_Dir = eDIR::UPLEFT;
	}
	else if (vCalPos.x < 0 && vCalPos.y == 0)
	{
		m_Dir = eDIR::LEFT;
	}
	else if (vCalPos.x < 0 && vCalPos.y < 0)
	{
		m_Dir = eDIR::DOWNLEFT;
	}
	else if (vCalPos.x == 0 && vCalPos.y < 0)
	{
		m_Dir = eDIR::DOWN;
	}
	else if (vCalPos.x > 0 && vCalPos.y < 0)
	{
		m_Dir = eDIR::DOWNRIGHT;
	}
	else if (vCalPos.x > 0 && vCalPos.y == 0)
	{
		m_Dir = eDIR::RIGHT;
	}
	

	vPos += vCalPos;
	GetFSM()->GetStateMachine()->Transform()->SetRelativePos(vPos);
	
	if (m_PrvDir != m_Dir)
	{
		Exit(); Enter();
	}

	if (KEY_NONE(KEY::W)&& KEY_NONE(KEY::A)&& KEY_NONE(KEY::S)&& KEY_NONE(KEY::D))
	{
		ChangeState(L"StandState");
	}
	
}

void CRunningState::Enter()
{
	m_Dir = GetFSM()->GetStateMachine()->GetOwner()->GetDir();
	m_PrvDir = m_Dir;
	m_Anim = GetFSM()->GetStateMachine()->Animator2D();

	switch (m_Dir)
	{	
	case eDIR::UP:
		m_Anim->Play(L"Running_Up");
		break;
	case eDIR::DOWN:
		m_Anim->Play(L"Running_Down");
		break;
	case eDIR::LEFT:
		m_Anim->Play(L"Running_Left");
		break;
	case eDIR::RIGHT:
		m_Anim->Play(L"Running_Right2");
		break;
	case eDIR::UPLEFT:
		m_Anim->Play(L"Running_Up");
		break;
	case eDIR::UPRIGHT:
		m_Anim->Play(L"Running_Up");
		break;
	case eDIR::DOWNLEFT:
		m_Anim->Play(L"Running_Down");
		break;
	case eDIR::DOWNRIGHT:
		m_Anim->Play(L"Running_Down");
		break;
	}
}

void CRunningState::Exit()
{
	GetFSM()->GetStateMachine()->GetOwner()->SetDir(m_Dir);
}

