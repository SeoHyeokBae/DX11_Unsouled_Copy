#pragma once
#include "UI.h"

#include "imgui_internal.h"
#include <Engine/CTexture.h>
#include <Engine/CAnim.h>

class CGameObject;

class AnimationEditorUI :
    public UI
{
private:
    ImVector<ImRect> m_vecRect;       // 드래그 저장
    vector<tAnimFrm> m_vecAnim;      // 스프라이트 출력용
    //vector<int>    m_vecSelect;

    Ptr<CTexture>  m_CurAtlas;

    ImVec2         m_CanvasLeftTop;
    ImVec2         m_Scrolling;
    ImVec2         m_MousePos;
    ImVec2         m_CenterPos;

    tAnimFrm       m_FrmInfo;

    CGameObject*   m_TargetObj;
    
    int            m_CanvasIdx;
    int            m_AnimIdx;

    float          m_fps;
    float          m_Wheelsz;

    bool           m_bSlice;
    bool           m_bTrim;
    bool           m_bGrid;
    bool           m_bSmart;

public:
    void DrawCanvas();

    void SelectAtlas(DWORD_PTR _ptr);
    void SetGameObj(CGameObject* _obj) { m_TargetObj = _obj; }
    void SelectSprite(char* _str);
    void MouseGrip(const ImVec2& _ioMousePos, const ImVec2& _canvasMousePos, 
        const ImVec2& _LT, const ImVec2& _RB, ImVector<ImVec2>& _points, const float _idx, const float _wheel);
   
    ImRect TrimAtlas(int _idx);
    void GridSlice(ImVector<ImVec2>& _points, BYTE& _row, BYTE& _col, UINT& _width, UINT& _height);
    void SmartSlice(ImVector<ImVec2>& _points);

    void SaveAnim(const wstring& _str);
    void LoadAnim(const wstring& _str);


public:
    virtual void render_update() override;
    virtual void Deactivate() override;

public:
    AnimationEditorUI();
    ~AnimationEditorUI();
};

