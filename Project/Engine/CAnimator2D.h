#pragma once
#include "CComponent.h"

#include "CTexture.h"
#include "CAnim.h"

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
    int GetAnimCount() { return m_mapAnim.size(); }

public:
    virtual void finaltick() override;

    void UpdateData();
    static void Clear();

    // _LeftTop, _SliceSize, _Offset : Pixel Unit
    void Create(const wstring& _strKey, Ptr<CTexture> _AtlasTex, Vec2 _LeftTop,
        Vec2 _SliceSize, Vec2 _OffsetSize, Vec2 _Background, int _FrmCount, float _FPS);
    void Create(const wstring& _strKey, Ptr<CTexture> _AtlasTex, const vector<tAnimFrm> _FrmInfo, int _FrmCount);
    void AddAnim(const wstring& _strKey, const wstring& _strPath);

    CAnim* FindAnim(const wstring& _strAnimName);
    void Play(const wstring& _strAnimName, bool _bRepeat = true);

    void GetAnimName(vector<string>& _Out);

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CAnimator2D);
public:
    CAnimator2D();
    CAnimator2D(const CAnimator2D& _OriginAnimator);
    ~CAnimator2D();
};

