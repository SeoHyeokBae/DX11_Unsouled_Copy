#include "pch.h"
#include "CMonsterScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

CMonsterScript::CMonsterScript()
	: CScript(MONSTERSCRIPT)
	, m_DetectRange(400.f)
	, m_Speed(50.f)
{
}

CMonsterScript::~CMonsterScript()
{
}

void CMonsterScript::begin()
{
	Animator2D()->AddAnim(L"Zombie_Stand_Left", L"anim\\Zombie_Stand_Left.anim");
	Animator2D()->AddAnim(L"Zombie_Stand_Right", L"anim\\Zombie_Stand_Right.anim");
	Animator2D()->AddAnim(L"Zombie_Running_Left", L"anim\\Zombie_Running_Left.anim");
	Animator2D()->AddAnim(L"Zombie_Running_Right", L"anim\\Zombie_Running_Right.anim");

	GetOwner()->GetShadow()->AddComponent(new CAnimator2D(*GetOwner()->Animator2D()));

	if (StateMachine())
	{
		StateMachine()->AddBlackboardData(L"DetectRange", BB_DATA::FLOAT, &m_DetectRange);
		StateMachine()->AddBlackboardData(L"Speed", BB_DATA::FLOAT, &m_Speed);

		// 플레이어를 찾아서 Object 타입으로 블랙보드에 기록한다.
		CGameObject* pPlayer = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Player");
		if (pPlayer)
		{
			StateMachine()->AddBlackboardData(L"TargetObject", BB_DATA::OBJECT, pPlayer);
		}

		if (nullptr != StateMachine()->GetFSM())
		{
			StateMachine()->GetFSM()->SetState(L"IdleState");
			Animator2D()->Play(L"Zombie_Stand_Right");
		}
	}
}

void CMonsterScript::tick()
{
}

void CMonsterScript::SaveToFile(FILE* _File)
{
}

void CMonsterScript::LoadFromFile(FILE* _File)
{
}

