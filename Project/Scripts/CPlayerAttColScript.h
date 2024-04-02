#pragma once
#include <Engine\CScript.h>

class CGameObject;

class CPlayerAttColScript :
    public CScript
{
private:
    CGameObject*    m_Owner;
    CCollider2D*    m_Collider;
    wstring         m_sCurState;
    float           m_fDuration;

public:
    void ColliderOn() { m_Collider->SetVisible(true); };
    void ColliderOff() { m_Collider->SetVisible(false); m_fDuration = 0.f; };

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CPlayerAttColScript);
public:
    CPlayerAttColScript();
    ~CPlayerAttColScript();
};

