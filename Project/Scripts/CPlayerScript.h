#pragma once
#include <Engine/CScript.h>

class CGameObject;

class CPlayerScript :
    public CScript
{
private:
    CGameObject*    m_HitBox;
    CGameObject*    m_AttCol;
    CGameObject*    m_TopBodyCol;
    wstring         m_CurState;
    float           m_Speed;
    float           m_AftTime;
    float           m_fHitTime;

    bool           m_bCanHit;
public:
    bool IsCanHit() { return m_bCanHit; }
    void OnCanHit() { m_bCanHit = true; }
    void OffCanHit() { m_bCanHit = false; }

    CGameObject* GetAttCol() { return m_AttCol; }
    CGameObject* GetBodyCol() { return m_TopBodyCol; }

    void CreateAftImg();
    void Damaged();

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

