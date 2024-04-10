#pragma once
#include <Engine\CScript.h>

class CGameObject;

class CBossNiugScript :
    public CScript
{
private:
    float           m_DetectRange;
    float           m_Speed;
    CGameObject*    m_AttCol;
    Ptr<CPrefab>    m_pHitCollider;
    bool            m_bAttacking;   // HitCollider »ý¼º
    bool            m_bHit;

public:
    void init();
    void Hit();

    bool IsAttacking() { return m_bAttacking;}
    void OnAttacking() { m_bAttacking = true; }
    void OffAttacking() { m_bAttacking = false; }

    bool IsHit() { return m_bHit; }
    void OnHit() { m_bHit = true; }
    void OffHit() { m_bHit = false; }

    CGameObject* GetAttCol() { return m_AttCol; }

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider);

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;
    CLONE(CBossNiugScript);
public:
    CBossNiugScript();
    ~CBossNiugScript();
};

