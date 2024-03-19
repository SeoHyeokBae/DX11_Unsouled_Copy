#pragma once
#include "UI.h"

#include <Engine/CTexture.h>

class TileMapEditorUI
	: public UI

{
private:
    Ptr<CTexture> m_CurAtlas;

public:
    virtual void render_update() override;
    virtual void Deactivate() override;

    void SelectAtlas(DWORD_PTR _ptr);

public:
    TileMapEditorUI();
    ~TileMapEditorUI();
};

