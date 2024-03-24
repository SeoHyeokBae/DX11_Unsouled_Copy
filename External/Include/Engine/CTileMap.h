#pragma once
#include "CRenderComponent.h"

class CStructuredBuffer;

enum class eTileType // 충돌 처리 순서로 스크립트 추가 
{
    COLLIDER,
    CLIFF,
    WATER,
    UPSTAIR,
    DOWNSTAIR,
    NONE,
    END,
};

struct tTileInfo
{
    Vec2        vLeftTopUV;
    int         bRender;
    eTileType   eType;
};


class CTileMap :
    public CRenderComponent
{
private:
    UINT                  m_FaceX;                 // 가로 타일 개수
    UINT                  m_FaceY;                 // 세로 타일 개수
                         
    Ptr<CTexture>         m_TileAtlas;
    Vec2                  m_vTilePixelSize;
    Vec2                  m_vSliceSizeUV;
    
    UINT                  m_MaxCol;
    UINT                  m_MaxRow;
                         
    UINT                  m_TileIdx;

    vector<tTileInfo>     m_vecTileInfo;
    CStructuredBuffer*    m_TileInfoBuffer;

    bool                  m_bRender;

public:
    void SetTileAtlas(Ptr<CTexture> _Atlas, Vec2 _TilePixelSize);
    Ptr<CTexture> GetTileAtlas() { return m_TileAtlas; }

    void SetFace(UINT _FaceX, UINT _FaceY);
    UINT GetFaceX() { return m_FaceX; }
    UINT GetFaceY() { return m_FaceY; }
    Vec2 GetTilePixelSize() { return m_vTilePixelSize; }
    vector<tTileInfo> GetInfoVec() { return m_vecTileInfo; }
    eTileType GetTileType(UINT _idx) { return m_vecTileInfo[_idx].eType; }

    void SetTileIndex(UINT _Row, UINT _Col, UINT _ImgIdx);
    void SetTileInfoVec(vector<tTileInfo>& _info) { m_vecTileInfo = _info; }

public:
    virtual void finaltick() override;
    virtual void UpdateData() override;
    virtual void render() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CTileMap);

public:
    CTileMap();
    CTileMap(const CTileMap& _OriginTileMap);
    ~CTileMap();
};

