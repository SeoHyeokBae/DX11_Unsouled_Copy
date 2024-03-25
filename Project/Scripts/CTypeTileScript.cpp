#include "pch.h"
#include "CTypeTileScript.h"


CTypeTileScript::CTypeTileScript()
	:CScript(TYPETILESCRIPT)
{
}

CTypeTileScript::~CTypeTileScript()
{
}

void CTypeTileScript::begin()
{
}

void CTypeTileScript::tick()
{
	//GetOwner()->
}

void CTypeTileScript::SaveToFile(FILE* _File)
{
}

void CTypeTileScript::LoadFromFile(FILE* _File)
{
}

void CTypeTileScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CTypeTileScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
	// 일단은 Collider 중심 좌표로
	// LT과 거리차 구하기
	Vec2 vOtherPos = _OtherObj->Transform()->GetRelativePos().XY() + _OtherCollider->GetOffsetPos();
	Vec2 vPos = GetOwner()->Transform()->GetRelativePos().XY() + _Collider->GetOffsetPos();
	Vec2 vSize = _Collider->GetOffsetScale();
	Vec2 vLT = vPos + Vec2(-vSize.x / 2.f, vSize.y / 2.f);
	Vec2 vRT = vPos + Vec2(vSize.x / 2.f, -vSize.y / 2.f);

	// idx 배열 접근 예외처리
	if ((vLT.x > vOtherPos.x || vRT.x < vOtherPos.x) ||
		(vLT.y < vOtherPos.y || vRT.y > vOtherPos.y))
	{
		return;
	}

	Vec2 vDis = vOtherPos - vLT;
	// IDX 찾기
	Vec2 vIdx = vDis / GetOwner()->TileMap()->GetTilePixelSize();
	vIdx = Vec2(abs(vIdx.x), abs(vIdx.y));
	UINT col = vIdx.x;
	UINT row = vIdx.y;
	UINT faceX = GetOwner()->TileMap()->GetFaceX();
	UINT faceY = GetOwner()->TileMap()->GetFaceY();
	UINT idx = row * faceX + col;

	// 해당 idx 타입 대입
	eTileType type = GetOwner()->TileMap()->GetTileType(idx);
	// 위 type으로 obj state 변화 부여
	// TODO
}

void CTypeTileScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
	eTileType type = eTileType::NONE;
}

