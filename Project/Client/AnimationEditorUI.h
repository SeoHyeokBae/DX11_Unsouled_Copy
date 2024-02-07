#pragma once
#include "UI.h"

#include "imgui_internal.h"
#include <Engine/CTexture.h>

class AnimationEditorUI :
    public UI
{
private:
    vector<ImRect> m_vecRect;
    Ptr<CTexture> m_CurAtlas;

    ImVec2 m_CanVasLeftTop;

    bool m_bOpen;

public:
    void DrawCanvas();

public:
    virtual void render_update() override;
    virtual void Deactivate() override;

public:
    AnimationEditorUI();
    ~AnimationEditorUI();
};

