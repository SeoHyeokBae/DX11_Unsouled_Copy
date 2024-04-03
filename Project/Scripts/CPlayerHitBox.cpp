#include "pch.h"
#include "CPlayerHitBox.h"


CPlayerHitBox::CPlayerHitBox()
	: CScript(PLAYERHITBOX)
{
}

CPlayerHitBox::~CPlayerHitBox()
{
}

void CPlayerHitBox::begin()
{

}

void CPlayerHitBox::tick()
{
}


void CPlayerHitBox::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{

}

void CPlayerHitBox::Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{

	Vec2 vVelocity = GetOwner()->GetParent()->Movement()->GetVelocity();
	vVelocity -= (vVelocity * 0.1f);
	GetOwner()->GetParent()->Movement()->SetVelocity(vVelocity);

	// Monster를 밀어냄
	// 좌 우 위 아래
	if (_OtherObj->GetName() == L"Zombie")
	{
		Vec2 vPos = GetOwner()->GetParent()->Transform()->GetRelativePos().XY() + _Collider->GetOffsetPos();
		Vec2 vSize = _Collider->GetOffsetScale();
		Vec2 vOtherPos = _OtherObj->Transform()->GetRelativePos().XY() + _OtherCollider->GetOffsetPos();
		Vec2 vOtherSize = _OtherCollider->GetOffsetScale();

		float len_x = fabs(vOtherPos.x - vPos.x);
		float scale_x = fabs(vOtherSize.x / 2.0f + vSize.x / 2.0f);
		float len_y = fabs(vOtherPos.y - vPos.y);
		float scale_y = fabs(vOtherSize.y / 2.0f + vSize.y / 2.0f);

		Vector3 playerPos = _OtherObj->Transform()->GetRelativePos();

		if (len_x < scale_x && len_y < scale_y) // col 사각형 안에 들어올시
		{
			if (scale_x - len_x < scale_y - len_y)  // 좌우
			{
				if (vOtherPos.x > vPos.x)			// Right
				{
					playerPos.x += (scale_x - len_x) + 1.f;
				}
				else								// Left
				{
					playerPos.x -= (scale_x - len_x) + 1.f;
				}
			}
			else									// 상하
			{
				if (vOtherPos.y > vPos.y)			// Up
				{
					playerPos.y += (scale_y - len_y) + 1.f;
				}
				else								// Down
				{
					playerPos.y -= (scale_y - len_y) + 1.f;
				}
			}
		}
		_OtherObj->Transform()->SetRelativePos(playerPos);
	}
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
