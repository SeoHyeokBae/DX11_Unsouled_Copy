#pragma once
#include "UI.h"
#include "imgui_internal.h"

#include <Engine/CTexture.h>

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

public:
    virtual void render_update() override;
    virtual void Deactivate() override;

    void SelectTile(ImVec2 _uvLT, ImVec2 _uvRB);
public:
    TileMapEditorUI();
    ~TileMapEditorUI();
};

