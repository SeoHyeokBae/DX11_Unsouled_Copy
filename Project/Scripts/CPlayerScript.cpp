#include "pch.h"
#include "CPlayerScript.h"
#include <Engine/CAssetMgr.h>

#include <Engine/CGameObject.h>
#include "CMissileScript.h"

#include <Engine/CMaterial.h>
#include <Engine/CRenderComponent.h>

#include <Engine/CMovement.h>
#include <Engine/CStateMachine.h>
//#include <Engine/CTileMap.h>

CPlayerScript::CPlayerScript()
	: CScript(PLAYERSCRIPT)
	, m_Speed(100.f)
	, m_Dir(eDIR::NONE)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Player Speed", &m_Speed);
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::begin()
{
	// 나중에 load 또는 prefab
	Animator2D()->AddAnim(L"StaminaOut_Stand_Up", L"anim\\StaminaOut_Stand_Up.anim");
	Animator2D()->AddAnim(L"StaminaOut_Walking_Up_Left", L"anim\\StaminaOut_Walking_Up_Left.anim");
	Animator2D()->AddAnim(L"StaminaOut_Walking_Up_Right", L"anim\\StaminaOut_Walking_Up_Right.anim");
	Animator2D()->AddAnim(L"StaminaOut_Walking_Up_Stand", L"anim\\StaminaOut_Walking_Up_Stand.anim");
	Animator2D()->AddAnim(L"StaminaOut_Stand_Down", L"anim\\StaminaOut_Stand_Down.anim");
	Animator2D()->AddAnim(L"StaminaOut_Walking_Down_Left", L"anim\\StaminaOut_Walking_Down_Left.anim");
	Animator2D()->AddAnim(L"StaminaOut_Walking_Down_Right", L"anim\\StaminaOut_Walking_Down_Right.anim");
	Animator2D()->AddAnim(L"StaminaOut_Walking_Down_Stand", L"anim\\StaminaOut_Walking_Down_Stand.anim");

	Animator2D()->AddAnim(L"Running_Down", L"anim\\Running_Down.anim");
	Animator2D()->AddAnim(L"Running_Up", L"anim\\Running_Up.anim");
	Animator2D()->AddAnim(L"Running_Left", L"anim\\Running_Left.anim");
	Animator2D()->AddAnim(L"Running_Right2", L"anim\\Running_Right2.anim");

	Animator2D()->AddAnim(L"Stand_Down", L"anim\\Stand_Down.anim");
	Animator2D()->AddAnim(L"Stand_Up", L"anim\\Stand_Up.anim");
	Animator2D()->AddAnim(L"Stand_Left", L"anim\\Stand_Left.anim");
	Animator2D()->AddAnim(L"Stand_Right", L"anim\\Stand_Right.anim");

	// Shadow 에 애니메이션 등록
	GetOwner()->GetShadow()->AddComponent(new CAnimator2D(*GetOwner()->Animator2D()));


	m_Missile = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"MissilePrefab");
	m_Missile = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\missile.pref", L"prefab\\missile.pref");

	// StateMachine 새팅
	if (StateMachine())
	{
		StateMachine()->AddBlackboardData(L"Speed", BB_DATA::FLOAT, &m_Speed);

		if (nullptr != StateMachine()->GetFSM())
		{
			StateMachine()->GetFSM()->SetState(L"StandState");
			Animator2D()->Play(L"Stand_Down");
		}
	}

	

	GetRenderComponent()->GetDynamicMaterial();
}

void CPlayerScript::tick()
{
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vRot = Transform()->GetRelativeRotation();

	if (KEY_PRESSED(KEY::W))
	{
		vPos.y += DT * m_Speed;
	}
	if (KEY_TAP(KEY::W) && KEY_NONE(KEY::A) && KEY_NONE(KEY::S) && KEY_NONE(KEY::D))
	{
		m_Dir = eDIR::UP;
		StateMachine()->GetFSM()->ChangeState(L"RunningState");
	}
	if (KEY_RELEASED(KEY::W))
	{
		if (KEY_NONE(KEY::D) && KEY_NONE(KEY::A))
		{
			//anim->Play(L"Stand_Up");
			m_Dir = eDIR::UP;
		}
	}

	if (KEY_PRESSED(KEY::S))
	{
		vPos.y -= DT * m_Speed;

	}
	if (KEY_TAP(KEY::S) && KEY_NONE(KEY::W) && KEY_NONE(KEY::A) && KEY_NONE(KEY::D))
	{
		m_Dir = eDIR::DOWN;
		StateMachine()->GetFSM()->ChangeState(L"RunningState");
	}
	if (KEY_RELEASED(KEY::S))
	{
		if (KEY_NONE(KEY::D) && KEY_NONE(KEY::A))
		{
			//anim->Play(L"Stand_Down");
			m_Dir = eDIR::DOWN;

		}
	}

	if (KEY_PRESSED(KEY::A))
	{
		vPos.x -= DT * m_Speed;
		if (KEY_TAP(KEY::W))
		{
			m_Dir = eDIR::UP;
			StateMachine()->GetFSM()->ChangeState(L"RunningState");
		}

		if (KEY_TAP(KEY::S))
		{
			m_Dir = eDIR::DOWN;
			StateMachine()->GetFSM()->ChangeState(L"RunningState");
		}

		if (KEY_TAP(KEY::D))
		{
			//anim->Play(L"Stand_Left");
			m_Dir = eDIR::LEFT;

		}

		if (KEY_RELEASED(KEY::D))
		{
			m_Dir = eDIR::LEFT;
			StateMachine()->GetFSM()->ChangeState(L"RunningState");
		}

		if (KEY_RELEASED(KEY::W) || KEY_RELEASED(KEY::S))
		{
			m_Dir = eDIR::LEFT;
			StateMachine()->GetFSM()->ChangeState(L"RunningState");
		}
	}
	if (KEY_TAP(KEY::A) && KEY_NONE(KEY::W) && KEY_NONE(KEY::S) && KEY_NONE(KEY::D))
	{
		m_Dir = eDIR::LEFT;
		StateMachine()->GetFSM()->ChangeState(L"RunningState");
	}
	if (KEY_RELEASED(KEY::A))
	{
		if (KEY_NONE(KEY::W) && KEY_NONE(KEY::S))
		{
			//anim->Play(L"Stand_Left");
			m_Dir = eDIR::LEFT;

		}
	}

	if (KEY_PRESSED(KEY::D))
	{
		vPos.x += DT * m_Speed;

		if (KEY_TAP(KEY::W))
		{
			m_Dir = eDIR::UP;
			StateMachine()->GetFSM()->ChangeState(L"RunningState");
		}

		if (KEY_TAP(KEY::S))
		{
			m_Dir = eDIR::DOWN;
			StateMachine()->GetFSM()->ChangeState(L"RunningState");
		}

		if (KEY_TAP(KEY::A))
		{
			//anim->Play(L"Stand_Right");
			m_Dir = eDIR::RIGHT;

		}

		if (KEY_RELEASED(KEY::A))
		{
			m_Dir = eDIR::RIGHT;
			StateMachine()->GetFSM()->ChangeState(L"RunningState");
		}

		if (KEY_RELEASED(KEY::W) || KEY_RELEASED(KEY::S))
		{
			m_Dir = eDIR::RIGHT;
			StateMachine()->GetFSM()->ChangeState(L"RunningState");
		}
	}
	if (KEY_TAP(KEY::D) && KEY_NONE(KEY::W) && KEY_NONE(KEY::S) && KEY_NONE(KEY::A))
	{
		m_Dir = eDIR::RIGHT;
		StateMachine()->GetFSM()->ChangeState(L"RunningState");
	}
	if (KEY_RELEASED(KEY::D))
	{
		if (KEY_NONE(KEY::W) && KEY_NONE(KEY::S) && KEY_NONE(KEY::A))
		{
			//anim->Play(L"Stand_Right");
			m_Dir = eDIR::RIGHT;
		}
	}

	if (KEY_NONE(KEY::W) && KEY_NONE(KEY::S) && KEY_NONE(KEY::A) && KEY_NONE(KEY::D))
	{
		StateMachine()->GetFSM()->ChangeState(L"StandState");
	}


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

void CPlayerScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{

}

void CPlayerScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
	if (_OtherObj->TileMap())
	{
		Vec2 vPos = GetOwner()->Transform()->GetRelativePos().XY();
		Vec2 TileObjHalfSize = _OtherObj->Transform()->GetRelativeScale().XY() / 2 ;
		Vec2 TileLT = _OtherObj->Transform()->GetRelativePos().XY() - TileObjHalfSize;


	}
}

void CPlayerScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}


void CPlayerScript::SaveToFile(FILE* _File)
{
	fwrite(&m_Speed, sizeof(float), 1, _File);
}

void CPlayerScript::LoadFromFile(FILE* _File)
{
	fread(&m_Speed, sizeof(float), 1, _File);
}