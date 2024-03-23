#pragma once
#include "UI.h"
#include "imgui_internal.h"

#include <Engine/CTexture.h>
#include <Engine/CTileMap.h>

enum class TILE_DRAW_MODE
{
    PAINT,
    FILL,
    ERASER,
    NONE,
};

class TileMapEditorUI
	: public UI

{
private:
    Ptr<CTexture>       m_CurSheet;
    TILE_DRAW_MODE      m_DrawMode;
    tTileInfo           m_Selected;
    vector<tTileInfo>   m_vecTileInfo; // ≈∏¿œ
    vector<eTileType>   m_vecType;
    bool                m_bChange;

public:
    virtual void render_update() override;
    virtual void Deactivate() override;

    void ClickEvent(int _faceX, int _faceY, int _row, int _col);
    void Clear(int _faceX, int faceY);
    void ChangeSheet(int _row, int _col);

public:
    TileMapEditorUI();
    ~TileMapEditorUI();
};

