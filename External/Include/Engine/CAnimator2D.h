#pragma once
#include "CComponent.h"

#include "CTexture.h"

class CAnim;

class CAnimator2D :
    public CComponent
{
private:
    map<wstring, CAnim*>     m_mapAnim;
    CAnim*                   m_CurAnim;
    wstring                  m_CurKey;
    bool                     m_bRepeat;

public:
    void SetCurAnim(CAnim* _anim) { m_CurAnim = _anim; }
    CAnim* GetCurAnim() { return m_CurAnim; }
    const wstring& GetCurKey() { return m_CurKey; }


public:
    virtual void finaltick() override;

    void UpdateData();
    static void Clear();

    // _LeftTop, _SliceSize, _Offset : Pixel Unit
    void Create(const wstring& _strKey, Ptr<CTexture> _AtlasTex, Vec2 _LeftTop,
        Vec2 _SliceSize, Vec2 _OffsetSize, Vec2 _Background, int _FrmCount, float _FPS);

    CAnim* FindAnim(const wstring& _strAnimName);
    void Play(const wstring& _strAnimName, bool _bRepeat = true);

    void GetAnimName(vector<string>& _Out);

    CLONE(CAnimator2D);
public:
    CAnimator2D();
    CAnimator2D(const CAnimator2D& _OriginAnimator);
    ~CAnimator2D();
};

