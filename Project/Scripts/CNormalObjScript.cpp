#include "pch.h"
#include "CNormalObjScript.h"

CNormalObjScript::CNormalObjScript()
	:CScript(NORMALOBJSCRIPT)
{
}

CNormalObjScript::~CNormalObjScript()
{
}

void CNormalObjScript::begin()
{
	GetRenderComponent()->GetDynamicMaterial();
	wstring name = GetOwner()->GetName();
	wstring path = L"texture\\" + name + L".png";
	MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(name, path));
	Ptr<CTexture> tex = CAssetMgr::GetInst()->FindAsset<CTexture>(name);
	Transform()->SetRelativeScale(Vec3(tex->GetWidth(), tex->GetHeight(), 1.f));

	GetOwner()->GetShadow()->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(name, path));
	GetOwner()->GetShadow()->Transform()->SetRelativeScale(Transform()->GetRelativeScale());
}

void CNormalObjScript::tick()
{
}

void CNormalObjScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{

}

void CNormalObjScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
	// Player or Monster을 밀어냄
	// 좌 우 위 아래
	Vec2 vPos = GetOwner()->Transform()->GetRelativePos().XY() + _Collider->GetOffsetPos();
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

void CNormalObjScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CNormalObjScript::SaveToFile(FILE* _File)
{
}

void CNormalObjScript::LoadFromFile(FILE* _File)
{
}
