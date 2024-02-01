#pragma once
#include "singleton.h"

#include "CTexture.h"

class CGameObject;
class CCamera;
class CLight2D;

class CStructuredBuffer;

class CRenderMgr :
    public CSingleton<CRenderMgr>
{
    SINGLE(CRenderMgr);
private:
    vector <CCamera*>       m_vecCam;   

    Ptr<CTexture>           m_PostProcessTex;

    CStructuredBuffer*      m_Light2DBuffer;
    vector<CLight2D*>       m_vecLight2D;

    list<tDebugShapeInfo>   m_DbgShapeInfo;
    CGameObject*            m_pDebugObj;

    bool                    m_DebugPosition;

    // NoiseTexture
    vector<Ptr<CTexture>>   m_vecNoiseTex;


public:
    void RegisterCamera(CCamera* _Cam, int _Idx);
    void AddDebugShapeInfo(const tDebugShapeInfo& _info)
    {
        m_DbgShapeInfo.push_back(_info);
    }

    void SetDebugPosition(bool _OnOff) { m_DebugPosition = _OnOff; }
    bool IsDebugPosition() { return m_DebugPosition; }

    void RegisterLight2D(CLight2D* _Light2D) { m_vecLight2D.push_back(_Light2D); }

    void CopyRenderTargetToPostProcessTarget(); // 렌더타켓을 후처리용으로 텍스처 복사
    Ptr<CTexture> GetPostProcessTex() { return m_PostProcessTex; }

public:
    void init();
    void tick();

private:
    void render();
    void render_debug();

    // 리소스 바인딩
    void UpdateData();

    // 리소스 클리어
    void Clear();
};

