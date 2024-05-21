#include "pch.h"
#include "CPlayerScript.h"
#include <Engine/CAssetMgr.h>

#include <Engine/CGameObject.h>
#include "CMissileScript.h"

#include <Engine/CMaterial.h>
#include <Engine/CRenderComponent.h>
#include <Engine/CMovement.h>
#include <Engine/CStateMachine.h>
#include <Engine/CPrefab.h>
#include <Engine/CAssetMgr.h>

#include "CAfterImageScript.h"
#include "CPlayerHitBox.h"
#include "CPlayerAttColScript.h"
#include "CChainSystemScript.h"
#include "CPlayerTopBodyScript.h"

CPlayerScript::CPlayerScript()
	: CScript(PLAYERSCRIPT)
	, m_HitBox (nullptr)
	, m_TopBodyCol(nullptr)
	, m_AttCol(nullptr)
	, m_Speed(100.f)
	, m_AftTime(0.0f)
	, m_fHitTime(0.f)
	, m_bHit(false)
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

	Animator2D()->AddAnim(L"Dash_Down", L"anim\\Dash_Down.anim");
	Animator2D()->AddAnim(L"Dash_DownRight", L"anim\\Dash_DownRight.anim");
	Animator2D()->AddAnim(L"Dash_DownRight2", L"anim\\Dash_DownRight2.anim");
	Animator2D()->AddAnim(L"Dash_Left", L"anim\\Dash_Left.anim");
	Animator2D()->AddAnim(L"Dash_Right", L"anim\\Dash_Right.anim");
	Animator2D()->AddAnim(L"Dash_Up", L"anim\\Dash_Up.anim");
	Animator2D()->AddAnim(L"Dash_UpRight", L"anim\\Dash_UpRight.anim");

	Animator2D()->AddAnim(L"DashAtt_Down", L"anim\\DashAtt_Down.anim");
	Animator2D()->AddAnim(L"DashAtt_Up", L"anim\\DashAtt_Up.anim");
	Animator2D()->AddAnim(L"DashAtt_Left", L"anim\\DashAtt_Left.anim");
	Animator2D()->AddAnim(L"DashAtt_Right", L"anim\\DashAtt_Right.anim");
	Animator2D()->AddAnim(L"DashAtt_DownLeft", L"anim\\DashAtt_DownLeft.anim");
	Animator2D()->AddAnim(L"DashAtt_DownRight", L"anim\\DashAtt_DownRight.anim");
	Animator2D()->AddAnim(L"DashAtt_UpLeft", L"anim\\DashAtt_UpLeft.anim");
	Animator2D()->AddAnim(L"DashAtt_UpRight", L"anim\\DashAtt_UpRight.anim");

	Animator2D()->AddAnim(L"Block_Down", L"anim\\Block_Down.anim");
	Animator2D()->AddAnim(L"Block_Up", L"anim\\Block_Up.anim");
	Animator2D()->AddAnim(L"Block_Left", L"anim\\Block_Left.anim");
	Animator2D()->AddAnim(L"Block_Right", L"anim\\Block_Right.anim");

	// Shadow 에 애니메이션 등록
	// 쉐도우 선 추가 -> 구조변경 필요 여기서 스크립트 추가해도
	GetOwner()->GetShadow()->AddComponent(new CAnimator2D(*GetOwner()->Animator2D()));

	// Attack Collider
	m_AttCol = new CGameObject;
	m_AttCol->SetName(L"Player_AttCol");
	m_AttCol->AddComponent(new CTransform);
	m_AttCol->AddComponent(new CCollider2D);
	m_AttCol->AddComponent(new CMeshRender);
	m_AttCol->AddComponent(new CPlayerAttColScript);
	m_AttCol->Transform()->SetRelativePos(Vec3(0.f, 20.f,0.f));
	m_AttCol->Collider2D()->SetVisible(true);
	m_AttCol->Collider2D()->SetOffsetScale(Vec2(35.f, 15.f));
	m_AttCol->Collider2D()->SetOffsetPos(Vec2(11.f, 0.f));
	GetOwner()->AddChild(m_AttCol);
	GamePlayStatic::SpawnGameObject(m_AttCol, 20);
	
	// HitBox Collider
	m_HitBox = new CGameObject;
	m_HitBox->SetName(L"Player_HitBox");
	m_HitBox->AddComponent(new CTransform);
	m_HitBox->AddComponent(new CCollider2D);
	m_HitBox->AddComponent(new CMeshRender);
	m_HitBox->AddComponent(new CPlayerHitBox);
	m_HitBox->Collider2D()->SetVisible(true);
	m_HitBox->Collider2D()->SetOffsetPos(Vec2(0.f, 20.f));
	m_HitBox->Collider2D()->SetOffsetScale(Vec2(13.f, 15.f));
	GetOwner()->AddChild(m_HitBox);
	GamePlayStatic::SpawnGameObject(m_HitBox, 21);

	// Top Body Collider
	m_TopBodyCol = new CGameObject;
	m_TopBodyCol->SetName(L"Player_TopBody");
	m_TopBodyCol->AddComponent(new CTransform);
	m_TopBodyCol->AddComponent(new CCollider2D);
	m_TopBodyCol->AddComponent(new CMeshRender);
	m_TopBodyCol->AddComponent(new CPlayerTopBodyScript);
	m_TopBodyCol->Collider2D()->SetVisible(true);
	m_TopBodyCol->Collider2D()->SetOffsetPos(Vec2(0.f, 20.f));
	m_TopBodyCol->Collider2D()->SetOffsetScale(Vec2(13.f, 15.f));
	GetOwner()->AddChild(m_TopBodyCol);
	GamePlayStatic::SpawnGameObject(m_TopBodyCol, 22);

	// StateMachine 세팅
	if (StateMachine())
	{
		StateMachine()->AddBlackboardData(L"Speed", BB_DATA::FLOAT, &m_Speed);
		StateMachine()->AddBlackboardData(L"Combo", BB_DATA::INT, &m_Speed);

		if (nullptr != StateMachine()->GetFSM())
		{
			StateMachine()->GetFSM()->SetState(L"StandState");
			GetOwner()->SetDir(eDIR::DOWN);
			Animator2D()->Play(L"Stand_Down");
		}
	}
	
	

	GetRenderComponent()->GetDynamicMaterial();
}



void CPlayerScript::tick()
{
	m_CurState = StateMachine()->GetFSM()->GetCurStateName();

	// 잔상 효과 이미지 정보 저장
	if (GetOwner()->IsAfterImgAct())
	{
		CreateAftImg();

		if (Animator2D()->GetCurAnim()->IsFinish())
		{
			GetOwner()->SetAfterImgAct(false);
		}
	}

	if (KEY_TAP(KEY::LBTN) && m_CurState != L"AttackState" && m_CurState != L"DashState" && m_CurState != L"DashaAttState")
	{
		StateMachine()->GetFSM()->ChangeState(L"AttackState");
	}

	if (KEY_TAP(KEY::LSHIFT) && m_CurState != L"BlockState")
	{
		StateMachine()->GetFSM()->ChangeState(L"BlockState");
	} 
	//|| (m_CurState != L"AttackState" && GetOwner()->GetScript<CChainSystemScript>()->IsChain()
	if ((KEY_TAP(KEY::RBTN) && m_CurState != L"DashState"))
	{
		StateMachine()->GetFSM()->ChangeState(L"DashState");
	}

	if (KEY_TAP(KEY::F) && m_CurState != L"AbsorbState")
	{
		StateMachine()->GetFSM()->ChangeState(L"AbsorbState");
	}


}

void CPlayerScript::CreateAftImg()
{
	// 잔상 스크립트 없음
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

void CPlayerScript::Damaged()
{
	// ToDo
	// 체력ㄷ감소 
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