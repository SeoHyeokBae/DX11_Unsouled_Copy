#pragma once
#include "UI.h"

#include "imgui_internal.h"
#include <Engine/CTexture.h>
#include <Engine/CAnim.h>

class AnimationEditorUI :
    public UI
{
private:
    vector<ImRect> m_vecRect;       // 드래그 저장
    vector<tAnimFrm> m_vecAnimRect;   // 스프라이트 출력용
    //vector<int>    m_vecSelect;

    Ptr<CTexture>  m_CurAtlas;

    ImVec2         m_CanvasLeftTop;
    ImVec2         m_Scrolling;
    ImVec2         m_MousePos;
    ImVec2         m_CenterPos;

    tAnimFrm       m_FrmInfo;
    
    int            m_SelectCanvasIdx;
    int            m_SelectAnimIdx;

    float          m_fps;
    float          m_Wheelsz;

    bool           m_bSlice;
    bool           m_bTrim;
    bool           m_bSmartSlice;

public:
    void DrawCanvas();
    void SelectAtlas(DWORD_PTR _ptr);
    ImRect TrimAtlas(int _idx);
    void SmartSlice(ImVector<ImVec2>& _points);

public:
    virtual void render_update() override;
    virtual void Deactivate() override;

public:
    AnimationEditorUI();
    ~AnimationEditorUI();
};

