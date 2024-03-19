#pragma once
#include "UI.h"

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

public:
    virtual void render_update() override;
    virtual void Deactivate() override;

public:
    TileMapEditorUI();
    ~TileMapEditorUI();
};

