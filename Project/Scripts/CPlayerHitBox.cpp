#include "pch.h"
#include "CPlayerHitBox.h"


CPlayerHitBox::CPlayerHitBox()
	: CScript(PLAYERHITBOX)
	, m_ColObj(nullptr)
{
}

CPlayerHitBox::~CPlayerHitBox()
{
}

void CPlayerHitBox::begin()
{
	m_ColObj = new CGameObject;
	m_ColObj->SetName(L"HitBox");
	m_ColObj->AddComponent(new CTransform);
	m_ColObj->AddComponent(new CCollider2D);
	m_ColObj->AddComponent(new CMeshRender);

	m_ColObj->Collider2D()->SetVisible(true);
	m_ColObj->Collider2D()->SetOffsetPos(Vec2(0.f, 20.f));
	m_ColObj->Collider2D()->SetOffsetScale(Vec2(15.f, 25.f));
	GetOwner()->AddChild(m_ColObj);

	GamePlayStatic::SpawnGameObject(m_ColObj, 0);
}

void CPlayerHitBox::tick()
{
}


void CPlayerHitBox::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CPlayerHitBox::Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CPlayerHitBox::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CPlayerHitBox::SaveToFile(FILE* _File)
{
}

void CPlayerHitBox::LoadFromFile(FILE* _File)
{
}
