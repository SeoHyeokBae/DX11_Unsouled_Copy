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
#include "CAfterImageScript.h"

CPlayerScript::CPlayerScript()
	: CScript(PLAYERSCRIPT)
	, m_Speed(100.f)
	, m_Chain(0)
	, m_fBlinkTime(0.f)
	, m_bYellow(false)
	, m_AftTime(0.0f)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Player Speed", &m_Speed);
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::begin()
{
	// ���߿� load �Ǵ� prefab
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

	Animator2D()->AddAnim(L"Dash_Down", L"anim\\Dash_Down.anim");
	Animator2D()->AddAnim(L"Dash_DownRight", L"anim\\Dash_DownRight.anim");
	Animator2D()->AddAnim(L"Dash_DownRight2", L"anim\\Dash_DownRight2.anim");
	Animator2D()->AddAnim(L"Dash_Left", L"anim\\Dash_Left.anim");
	Animator2D()->AddAnim(L"Dash_Right", L"anim\\Dash_Right.anim");
	Animator2D()->AddAnim(L"Dash_Up", L"anim\\Dash_Up.anim");
	Animator2D()->AddAnim(L"Dash_UpRight", L"anim\\Dash_UpRight.anim");

	// Shadow �� �ִϸ��̼� ���
	// ������ �� �߰� -> �������� �ʿ�
	GetOwner()->GetShadow()->AddComponent(new CAnimator2D(*GetOwner()->Animator2D()));


	m_Missile = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"MissilePrefab");
	m_Missile = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\missile.pref", L"prefab\\missile.pref");

	// StateMachine ����
	if (StateMachine())
	{
		StateMachine()->AddBlackboardData(L"Speed", BB_DATA::FLOAT, &m_Speed);

		if (nullptr != StateMachine()->GetFSM())
		{
			GetOwner()->SetDir(eDIR::DOWN);
			StateMachine()->GetFSM()->SetState(L"StandState");
			Animator2D()->Play(L"Stand_Down");
		}
	}

	

	GetRenderComponent()->GetDynamicMaterial();
}



void CPlayerScript::tick()
{
	//Vec3 vPos = Transform()->GetRelativePos();
	//Vec3 vRot = Transform()->GetRelativeRotation();

	m_CurState = StateMachine()->GetFSM()->GetCurStateName();



	// �ܻ� ȿ�� �̹��� ���� ����
	if (GetOwner()->IsAfterImgAct())
	{
		CreateAftImg();

		if (Animator2D()->GetCurAnim()->IsFinish())
		{
			GetOwner()->SetAfterImgAct(false);
		}
	}

	if (KEY_TAP(KEY::LBTN) && m_CurState != L"AttackState")
	{
		StateMachine()->GetFSM()->ChangeState(L"AttackState");
	}

	if (KEY_TAP(KEY::RBTN) && m_CurState != L"DashState")
	{
		StateMachine()->GetFSM()->ChangeState(L"DashState");
	}

	if (KEY_TAP(KEY::F) && m_CurState != L"AbsorbState")
	{
		StateMachine()->GetFSM()->ChangeState(L"AbsorbState");
	}

	//Transform()->SetRelativePos(vPos);
	//Transform()->SetRelativeRotation(vRot);
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

void CPlayerScript::CreateAftImg()
{
	// �ܻ� ��ũ��Ʈ ����
	assert(GetOwner()->GetScript<CAfterImageScript>());

	Vec3 vPos = Transform()->GetRelativePos();

	if (m_AftTime == 0.f)
	{
		int idx = GetOwner()->GetScript<CAfterImageScript>()->GetCurIdx();
		if (idx % 3 < 3)
		{
			int animidx = Animator2D()->GetCurAnim()->GetCurFrmIdx();

			tAnimFrm frm = Animator2D()->GetCurAnim()->GetCurFrmInfo(animidx);
			Ptr<CTexture> tex = Animator2D()->GetCurAnim()->GetAtalsTex();

			GetOwner()->GetScript<CAfterImageScript>()->CreateAfterImg(tex, vPos, frm);

			idx++;
			GetOwner()->GetScript<CAfterImageScript>()->SetCurIdx(idx);
		}
	}

	m_AftTime += DT;
	if (m_AftTime >= 0.04f)
	{
		m_AftTime = 0.f;
		int idx = GetOwner()->GetScript<CAfterImageScript>()->GetCurIdx();
		if (idx >= 50)
			return;

		int animidx = Animator2D()->GetCurAnim()->GetCurFrmIdx();

		tAnimFrm frm = Animator2D()->GetCurAnim()->GetCurFrmInfo(animidx);
		Ptr<CTexture> tex = Animator2D()->GetCurAnim()->GetAtalsTex();

		GetOwner()->GetScript<CAfterImageScript>()->CreateAfterImg(tex, vPos, frm);

		idx++;
		GetOwner()->GetScript<CAfterImageScript>()->SetCurIdx(idx);
	}
}

void CPlayerScript::SaveToFile(FILE* _File)
{
	fwrite(&m_Speed, sizeof(float), 1, _File);
}

void CPlayerScript::LoadFromFile(FILE* _File)
{
	fread(&m_Speed, sizeof(float), 1, _File);
}