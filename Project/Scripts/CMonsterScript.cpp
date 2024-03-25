#include "pch.h"
#include "CMonsterScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

CMonsterScript::CMonsterScript()
	: CScript(MONSTERSCRIPT)
	, m_DetectRange(400.f)
	, m_Speed(200.f)
{
}

CMonsterScript::~CMonsterScript()
{
}

void CMonsterScript::begin()
{
	Ptr<CTexture> pAltasTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\link.png", L"texture\\link.png");
	Animator2D()->Create(L"IDLE_UP", pAltasTex, Vec2(0.f, 260.f), Vec2(120.f, 130.f), Vec2(0.f, 0.f), Vec2(200.f, 200.f), 1, 10);
	Animator2D()->Create(L"IDLE_DOWN", pAltasTex, Vec2(0.f, 0.f), Vec2(120.f, 130.f), Vec2(0.f, 0.f), Vec2(200.f, 200.f), 3, 10);
	Animator2D()->Create(L"IDLE_LEFT", pAltasTex, Vec2(0.f, 130.f), Vec2(120.f, 130.f), Vec2(0.f, 0.f), Vec2(200.f, 200.f), 3, 10);
	Animator2D()->Create(L"IDLE_RIGHT", pAltasTex, Vec2(0.f, 390.f), Vec2(120.f, 130.f), Vec2(0.f, 0.f), Vec2(200.f, 200.f), 3, 10);
	Animator2D()->Play(L"IDLE_RIGHT");
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
		}
	}
}

void CMonsterScript::tick()
{


	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vRot = Transform()->GetRelativeRotation();

	float limity = 5000.f;
	float limitz = 1500.f;
	if (0 <= GetOwner()->Transform()->GetRelativePos().y)
		vPos.z = (1.f / limity) * 1500;
	if (4 == GetOwner()->GetLayerIdx())
	{
		vPos.z = (vPos.y / limity) * 1500 + 50.f;
	}
	else
		vPos.z = (vPos.y / limity) * 1500;

	Transform()->SetRelativePos(vPos);
	Transform()->SetRelativeRotation(vRot);
}

void CMonsterScript::SaveToFile(FILE* _File)
{
}

void CMonsterScript::LoadFromFile(FILE* _File)
{
}

