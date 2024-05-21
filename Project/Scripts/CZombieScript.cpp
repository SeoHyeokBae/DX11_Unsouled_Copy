#include "pch.h"
#include "CZombieScript.h"

#include "CPlayerScript.h"
#include "CEffectScript.h"

CZombieScript::CZombieScript()
	:CScript(ZOMBIESCRIPT)
	, m_Hp(30)
	, m_bDamaged(false)
{
}

CZombieScript::~CZombieScript()
{
}


void CZombieScript::init()
{
	Animator2D()->AddAnim(L"Stand_Left", L"anim\\Zombie_Stand_Left.anim");
	Animator2D()->AddAnim(L"Stand_Right", L"anim\\Zombie_Stand_Right.anim");
	Animator2D()->AddAnim(L"Running_Left", L"anim\\Zombie_Running_Left.anim");
	Animator2D()->AddAnim(L"Running_Right", L"anim\\Zombie_Running_Right.anim");
}

void CZombieScript::begin()
{
	init();

	// 그림자에 애니메이션 등록
	GetOwner()->GetShadow()->AddComponent(new CAnimator2D(*GetOwner()->Animator2D()));

	if (nullptr != StateMachine()->GetFSM())
	{
		StateMachine()->GetFSM()->SetState(L"IdleState");
		Animator2D()->Play(L"Stand_Right");
	}
}

void CZombieScript::tick()
{
	
}

void CZombieScript::Damaged()
{
	// ToDo
	m_bDamaged = true;
	CEffectScript* effect = GetOwner()->GetScript<CEffectScript>();
	effect->OnEffect(eEffectStatus::SWORD_SPARK);
	effect->OnRed();
}

void CZombieScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CZombieScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{

}

void CZombieScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CZombieScript::SaveToFile(FILE* _File)
{
}

void CZombieScript::LoadFromFile(FILE* _File)
{
}
