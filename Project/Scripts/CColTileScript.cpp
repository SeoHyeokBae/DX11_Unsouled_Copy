#include "pch.h"
#include "CColTileScript.h"

CColTileScript::CColTileScript()
	: CScript(COLTILESCRIPT)
{
}

CColTileScript::~CColTileScript()
{
}

void CColTileScript::begin()
{
}

void CColTileScript::tick()
{
}


void CColTileScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
	// Player or Monster�� �о
	// �� �� �� �Ʒ�
	Vec2 vPos = GetOwner()->Transform()->GetRelativePos().XY() + _Collider->GetOffsetPos();
	Vec2 vSize = _Collider->GetOffsetScale();
	Vec2 vOtherPos = _OtherObj->Transform()->GetRelativePos().XY() + _OtherCollider->GetOffsetPos();
	Vec2 vOtherSize = _OtherCollider->GetOffsetScale();

	float len_x = fabs(vOtherPos.x - vPos.x);
	float scale_x = fabs(vOtherSize.x / 2.0f + vSize.x / 2.0f);
	float len_y = fabs(vOtherPos.y - vPos.y);
	float scale_y = fabs(vOtherSize.y / 2.0f + vSize.y / 2.0f);

	Vector3 playerPos = _OtherObj->Transform()->GetRelativePos();

	if (len_x < scale_x && len_y < scale_y) // col �簢�� �ȿ� ���ý�
	{
		if (scale_x - len_x < scale_y - len_y)  // �¿�
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
		else									// ����
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

void CColTileScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CColTileScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CColTileScript::SaveToFile(FILE* _File)
{
}

void CColTileScript::LoadFromFile(FILE* _File)
{
}