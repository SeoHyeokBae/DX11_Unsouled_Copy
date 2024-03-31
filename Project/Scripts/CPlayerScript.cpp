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
	, m_Chain(0)
	, m_fBlinkTime(0.f)
	, m_bYellow(false)
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

	Animator2D()->AddAnim(L"Absorb_Stand_Down", L"anim\\Absorb_Stand_Down.anim");
	Animator2D()->AddAnim(L"Absorb_Stand_Up", L"anim\\Absorb_Stand_Up.anim");
	Animator2D()->AddAnim(L"Absorb_Stand_Left", L"anim\\Absorb_Stand_Left.anim");
	Animator2D()->AddAnim(L"Absorb_Stand_Right", L"anim\\Absorb_Stand_Right.anim");
	Animator2D()->AddAnim(L"Absorb_Walking_Down", L"anim\\Absorb_Walking_Down.anim");
	Animator2D()->AddAnim(L"Absorb_Walking_Up", L"anim\\Absorb_Walking_Up.anim");
	Animator2D()->AddAnim(L"Absorb_Walking_Left", L"anim\\Absorb_Walking_Left.anim");
	Animator2D()->AddAnim(L"Absorb_Walking_Right", L"anim\\Absorb_Walking_Right.anim");

	Animator2D()->AddAnim(L"BAttack01_Down", L"anim\\BAttack01_Down.anim");
	Animator2D()->AddAnim(L"BAttack01_Up", L"anim\\BAttack01_Up.anim");
	Animator2D()->AddAnim(L"BAttack01_Left", L"anim\\BAttack01_Left.anim");
	Animator2D()->AddAnim(L"BAttack01_Right", L"anim\\BAttack01_Right.anim");

	Animator2D()->AddAnim(L"BAttack02_Down", L"anim\\BAttack02_Down.anim");
	Animator2D()->AddAnim(L"BAttack02_Up", L"anim\\BAttack02_Up.anim");
	Animator2D()->AddAnim(L"BAttack02_Left", L"anim\\BAttack02_Left.anim");
	Animator2D()->AddAnim(L"BAttack02_Right", L"anim\\BAttack02_Right.anim");

	Animator2D()->AddAnim(L"BAttack03_Down", L"anim\\BAttack03_Down.anim");
	Animator2D()->AddAnim(L"BAttack03_Up", L"anim\\BAttack03_Up.anim");
	Animator2D()->AddAnim(L"BAttack03_Left", L"anim\\BAttack03_Left.anim");
	Animator2D()->AddAnim(L"BAttack03_Right", L"anim\\BAttack03_Right.anim");

	// Shadow 에 애니메이션 등록
	GetOwner()->GetShadow()->AddComponent(new CAnimator2D(*GetOwner()->Animator2D()));


	m_Missile = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"MissilePrefab");
	m_Missile = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\missile.pref", L"prefab\\missile.pref");

	// StateMachine 새팅
	if (StateMachine())
	{
		StateMachine()->AddBlackboardData(L"Speed", BB_DATA::FLOAT, &m_Speed);
		StateMachine()->AddBlackboardData(L"Chain", BB_DATA::INT,  &m_Chain);

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
	m_CurState = StateMachine()->GetFSM()->GetCurStateName();

	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vRot = Transform()->GetRelativeRotation();

	if (KEY_TAP(KEY::LBTN) && m_CurState != L"AttackState")
	{
		StateMachine()->GetFSM()->ChangeState(L"AttackState");
	}

	if (KEY_TAP(KEY::F) && m_CurState != L"AbsorbState")
	{
		StateMachine()->GetFSM()->ChangeState(L"AbsorbState");
	}


	// 채인발동시 깜빡임
	m_Chain = *((int*)StateMachine()->GetBlackboardData(L"Chain"));
	if (m_Chain)
	{
		m_fBlinkTime += DT;
		if (m_fBlinkTime >= 0.03f) // interval
		{
			if (!m_bYellow)
			{
				m_bYellow = true;
				GetRenderComponent()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_3, 1);
			}
			else
			{
				m_bYellow = false;
				GetRenderComponent()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_3, 0);
			}

			m_fBlinkTime = 0.f;
		}
	}
	else
	{
		m_bYellow = false;
		GetRenderComponent()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_3, 0);
	}

	// y위치에 따른 z축 정렬
	float limity = 5000.f;
	float limitz = 1500.f;
	if (0 <= vPos.y)
		vPos.z = (1.f / limity) * 1500;

	if (4 == GetOwner()->GetLayerIdx())
		vPos.z = (vPos.y / limity) * 1500 + 50.f;
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