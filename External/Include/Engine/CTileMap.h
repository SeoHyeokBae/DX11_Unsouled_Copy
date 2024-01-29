#pragma once
#include "CRenderComponent.h"
class CTileMap :
    public CRenderComponent
{
private:
    UINT            m_FaceX;                 // 가로 타일 개수
    UINT            m_FaceY;                 // 세로 타일 개수
    Vec2            m_vTileRenderSize;       // 타일 1칸 사이즈

    Vec3            m_vTileMapWorldPos;

    Ptr<CTexture>   m_TileAtlas;
    Vec2            m_vTilePixelSize;

    UINT            m_MaxCol;
    UINT            m_MaxRow;

    UINT            m_TileIdx;



public:
    void SetTileAtlas(Ptr<CTexture> _Atlas, Vec2 _TilePixelSize);
    Ptr<CTexture> GetTileAtlas() { return m_TileAtlas; }

public:
    virtual void finaltick() override;
    virtual void UpdateData() override;
    virtual void render() override;


public:
    CTileMap();
    ~CTileMap();
};

