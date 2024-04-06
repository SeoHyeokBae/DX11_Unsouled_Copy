#include "pch.h"
#include "CBossNiugScript.h"

CBossNiugScript::CBossNiugScript()
	: CScript(BOSSNIUGSCRIPT)
{
}

CBossNiugScript::~CBossNiugScript()
{
}

void CBossNiugScript::init()
{
	Animator2D()->AddAnim(L"Niug_Running_Left", L"anim\\Niug_Running_Left.anim");
	Animator2D()->AddAnim(L"Niug_Running_Right", L"anim\\Niug_Running_Right.anim");
}

void CBossNiugScript::begin()
{
	init();

	// 그림자에 애니메이션 등록
	GetOwner()->GetShadow()->AddComponent(new CAnimator2D(*GetOwner()->Animator2D()));

	GetRenderComponent()->GetDynamicMaterial();

	if (nullptr != StateMachine()->GetFSM())
	{
		StateMachine()->GetFSM()->SetState(L"Niug_Running_Right");
		Animator2D()->Play(L"Niug_Running_Right");
	}
}

void CBossNiugScript::tick()
{
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
