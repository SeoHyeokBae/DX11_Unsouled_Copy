#pragma once
#include <Engine/CScript.h>

class CPlayerScript :
    public CScript
{
private:
    Ptr<CPrefab>    m_Missile;
    wstring         m_CurState;

    float           m_Speed;

    float           m_AftTime;
    // Chain ½Ã ±ôºýÀÓ
    int             m_Chain;
    float           m_fBlinkTime;
    bool            m_bYellow;

public:
    void CreateAftImg();

public:
    virtual void tick() override;
    virtual void begin() override;
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider);

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;
    CLONE(CPlayerScript);
public:
    CPlayerScript();
    ~CPlayerScript();
};

