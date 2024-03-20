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
    ImRect              m_Selected;
    vector<ImRect>      m_vecTile;
    vector<tTileInfo>   m_vecTileInfo;
    bool                m_bChange;

public:
    virtual void render_update() override;
    virtual void Deactivate() override;

    void SetTileIndex(int _row, int _col, int _faceX);
    void Clear(int _faceX, int faceY);
public:
    TileMapEditorUI();
    ~TileMapEditorUI();
};

