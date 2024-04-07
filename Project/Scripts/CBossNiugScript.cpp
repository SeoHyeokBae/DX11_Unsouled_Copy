#include "pch.h"
#include "CBossNiugScript.h"

#include "CBossNiugAttColScript.h"
#include "../Client/CNiug_BasicAttState.h"

#include "CHitColliderScript.h"

CBossNiugScript::CBossNiugScript()
	: CScript(BOSSNIUGSCRIPT)
	, m_AttCol(nullptr)
	, m_bAttacking(false)
	, m_pHitCollider(nullptr)
{
}

CBossNiugScript::~CBossNiugScript()
{
}

void CBossNiugScript::init()
{
	Animator2D()->AddAnim(L"Niug_Running_Left", L"anim\\Niug_Running_Left.anim");
	Animator2D()->AddAnim(L"Niug_Running_Right", L"anim\\Niug_Running_Right.anim");

	Animator2D()->AddAnim(L"Niug_BasicAtt01_Left", L"anim\\Niug_BasicAtt01_Left.anim");
	Animator2D()->AddAnim(L"Niug_BasicAtt01_Right", L"anim\\Niug_BasicAtt01_Right.anim");
	Animator2D()->AddAnim(L"Niug_BasicAtt02_Left", L"anim\\Niug_BasicAtt02_Left.anim");
	Animator2D()->AddAnim(L"Niug_BasicAtt02_Right", L"anim\\Niug_BasicAtt02_Right.anim");
	Animator2D()->AddAnim(L"Niug_BasicAtt03_Left", L"anim\\Niug_BasicAtt03_Left.anim");
	Animator2D()->AddAnim(L"Niug_BasicAtt03_Right", L"anim\\Niug_BasicAtt03_Right.anim");

	Animator2D()->AddAnim(L"Niug_RageOfIsno_Left", L"anim\\Niug_RageOfIsno_Left.anim");
	Animator2D()->AddAnim(L"Niug_RageOfIsno_Right", L"anim\\Niug_RageOfIsno_Right.anim");
	
	// hit collider 프리펩
	m_pHitCollider = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Niug_HitCollider.pref", L"prefab\\Niug_HitCollider.pref");
	CGameObject* obj = m_pHitCollider->GetProtoGameObj();
	obj->Transform()->SetRelativeScale(Vec3(300.f, 300.f, 1.f));
	obj->Collider2D()->SetOffsetScale(Vec2(30.f, 50.f));
	obj->Collider2D()->SetVisible(true);
	obj->GetScript<CHitColliderScript>()->SetSelf(GetOwner());
	//obj->GetScript<CHitColliderScript>()->SetDuration(2.5f);

	// Att Collider
	m_AttCol = new CGameObject;
	m_AttCol->SetName(L"AttCol");
	m_AttCol->AddComponent(new CTransform);
	m_AttCol->AddComponent(new CCollider2D);
	m_AttCol->AddComponent(new CMeshRender);
	m_AttCol->AddComponent(new CBossNiugAttColScript);
	m_AttCol->Collider2D()->SetVisible(true);
	m_AttCol->Transform()->SetRelativePos(Vec3(0.f, 20.f,0.f));
	m_AttCol->Collider2D()->SetOffsetPos(Vec2(25.f, 0.f));
	m_AttCol->Collider2D()->SetOffsetScale(Vec2(50.f, 30.f));
	GetOwner()->AddChild(m_AttCol);
	GamePlayStatic::SpawnGameObject(m_AttCol, 22);
}

void CBossNiugScript::begin()
{
	init();

	// 그림자에 애니메이션 등록
	GetOwner()->GetShadow()->AddComponent(new CAnimator2D(*GetOwner()->Animator2D()));

	GetRenderComponent()->GetDynamicMaterial();

	if (nullptr != StateMachine()->GetFSM())
	{
		StateMachine()->GetFSM()->SetState(L"RunningState");
		Animator2D()->Play(L"Niug_Running_Right");
		GetOwner()->SetDir(eDIR::RIGHT);
	}
}

void CBossNiugScript::tick()
{
	CGameObject* pTarget = (CGameObject*)StateMachine()->GetBlackboardData(L"TargetObject");
	if (m_bAttacking)
	{
		m_bAttacking = false;
		Instantiate(m_pHitCollider, pTarget->Transform()->GetRelativePos(), 21);
	}


}

void CBossNiugScript::OnHit()
{
	//CNiug_BasicAttState* state = (CNiug_BasicAttState*)StateMachine()->GetFSM()->FindState(L"BasicAttState");
	//state->AddComboCount();
}


void CBossNiugScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
	
}

void CBossNiugScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
	
}

void CBossNiugScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CBossNiugScript::SaveToFile(FILE* _File)
{
}

void CBossNiugScript::LoadFromFile(FILE* _File)
{
}
