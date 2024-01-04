#pragma once
#include "CEntity.h"

#include "CTexture.h"

// 프레임 정보
struct tAnimFrm
{
    Vec2    vLeftTop;
    Vec2    vSlice;
    Vec2    vOffset;
    float   Duration;
};

class CAnimator2D;

class CAnim :
    public CEntity
{
private:
    CAnimator2D*        m_Animator;

    vector<tAnimFrm>    m_vecFrm;
    int                 m_CurFrmIdx;
    bool                m_bFinish;

    Ptr<CTexture>       m_AtlasTex;

public:
    void Create(CAnimator2D* _Animator, Ptr<CTexture> _Atlas, Vec2 _vLeftTop
        , Vec2 _vSliceSize, Vec2 _vOffset, int _FrmCount, float _FPS);

public:
    CAnim();
    ~CAnim();
};

