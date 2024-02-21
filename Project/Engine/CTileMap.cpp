#include "pch.h"
#include "CTileMap.h"

#include "CStructuredBuffer.h"

#include "CAssetMgr.h"
#include "CMesh.h"

#include "CTransform.h"

CTileMap::CTileMap()
	: CRenderComponent(COMPONENT_TYPE::TILEMAP)
	, m_FaceX(2)
	, m_FaceY(2)
	, m_vTileRenderSize(Vec2(128.f, 128.f))
	, m_TileInfoBuffer(nullptr)
{
	SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"TileMapMtrl"));

	m_TileInfoBuffer = new CStructuredBuffer;

	SetFace(m_FaceX, m_FaceY);
}

CTileMap::CTileMap(const CTileMap& _OriginTileMap)
	: CRenderComponent(_OriginTileMap)
	, m_FaceX(_OriginTileMap.m_FaceX)
	, m_FaceY(_OriginTileMap.m_FaceY)
	, m_vTileRenderSize(_OriginTileMap.m_vTileRenderSize)
	, m_vTileMapWorldPos(_OriginTileMap.m_vTileMapWorldPos)
	, m_TileAtlas(_OriginTileMap.m_TileAtlas)
	, m_vTilePixelSize(_OriginTileMap.m_vTilePixelSize)
	, m_vSliceSizeUV(_OriginTileMap.m_vSliceSizeUV)
	, m_MaxCol(_OriginTileMap.m_MaxCol)
	, m_MaxRow(_OriginTileMap.m_MaxRow)
	, m_vecTileInfo(_OriginTileMap.m_vecTileInfo)
	, m_TileInfoBuffer(nullptr)
{
	if (nullptr != _OriginTileMap.m_TileInfoBuffer)
	{
		m_TileInfoBuffer = _OriginTileMap.m_TileInfoBuffer->Clone();
	}
}

CTileMap::~CTileMap()
{
	if (nullptr != m_TileInfoBuffer)
		delete m_TileInfoBuffer;
}



void CTileMap::finaltick()
{
	// (타일 개수 * 타일 사이즈) 로 사이즈를 변경처리한다.
	Vec3 vTileMapSize = Vec3(m_FaceX * m_vTileRenderSize.x, m_FaceY * m_vTileRenderSize.y, 1.f);
	Transform()->SetRelativeScale(vTileMapSize);
}


void CTileMap::render()
{
	// 재질에 아틀라스 텍스쳐 전달.
	GetMaterial()->SetTexParam(TEX_0, m_TileAtlas);

	// 타일의 가로 세로 개수
	GetMaterial()->SetScalarParam(INT_0, m_FaceX);
	GetMaterial()->SetScalarParam(INT_1, m_FaceY);

	// 아틀라스 이미지에서 타일 1개의 자르는 사이즈(UV 기준)
	GetMaterial()->SetScalarParam(VEC2_0, m_vSliceSizeUV);

	// 각 타일 정보를 구조화 버퍼로 이동
	m_TileInfoBuffer->SetData(m_vecTileInfo.data(), m_vecTileInfo.size());

	// 타일 구조화 버퍼를 t20 에 바인딩
	m_TileInfoBuffer->UpdateData(20);

	// 재질 업데이트

	GetMaterial()->UpdateData();

	Transform()->UpdateData();

	GetMesh()->render();
}

void CTileMap::UpdateData()
{
}

void CTileMap::SetTileAtlas(Ptr<CTexture> _Atlas, Vec2 _TilePixelSize)
{
	m_TileAtlas = _Atlas;
	m_vTilePixelSize = _TilePixelSize;

	m_MaxCol = m_TileAtlas->GetWidth() / (UINT)m_vTilePixelSize.x;
	m_MaxRow = m_TileAtlas->GetHeight() / (UINT)m_vTilePixelSize.y;

	m_vSliceSizeUV = Vec2(m_vTilePixelSize.x / m_TileAtlas->GetWidth()
		, m_vTilePixelSize.y / m_TileAtlas->GetHeight());
}

void CTileMap::SetFace(UINT _FaceX, UINT _FaceY)
{
	m_FaceX = _FaceX;
	m_FaceY = _FaceY;

	vector<tTileInfo> vecTemp;
	m_vecTileInfo.swap(vecTemp);
	m_vecTileInfo.resize(_FaceX * _FaceY);

	m_TileInfoBuffer->Create(sizeof(tTileInfo), _FaceX * _FaceY, SB_TYPE::READ_ONLY, true);
}

void CTileMap::SetTileIndex(UINT _Row, UINT _Col, UINT _ImgIdx)	// 행 , 렬 , 사용할 이미지 번호
{
	if (nullptr == m_TileAtlas)
		return;

	UINT idx = _Row * m_FaceX + _Col;

	// 렌더링할 타일 정보
	UINT iRow = _ImgIdx / m_MaxCol;
	UINT iCol = _ImgIdx % m_MaxCol;

	m_vecTileInfo[idx].vLeftTopUV = Vec2((iCol * m_vTilePixelSize.x) / m_TileAtlas->GetWidth()
		, (iRow * m_vTilePixelSize.y) / m_TileAtlas->GetHeight());

	m_vecTileInfo[idx].bRender = 1;
}