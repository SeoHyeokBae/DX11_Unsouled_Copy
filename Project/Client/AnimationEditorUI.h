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

    ImVec2 m_CanvasLeftTop;
    ImVec2 m_Scrolling;
    ImVec2 m_MousePos;
    ImVec2 m_CenterPos;
    float m_Wheelsz;
    bool m_bOpen;

    bool m_bSlice;
    bool m_bTrim;
    bool m_bSmartSlice;

public:
    void DrawCanvas();
    void AtlasSelect(DWORD_PTR _ptr);

public:
    virtual void render_update() override;
    virtual void Deactivate() override;

public:
    AnimationEditorUI();
    ~AnimationEditorUI();
};

