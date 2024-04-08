#pragma once
#include <Engine\CScript.h>

class CGameObject;

class CPlayerAttackScript :
    public CScript
{
private:
    eDIR            m_Dir;
    CGameObject*    m_AttCol;
    Ptr<CPrefab>    m_preAttCol;
    bool            m_bAttacking;


public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CPlayerAttackScript);
public:
    CPlayerAttackScript();
    ~CPlayerAttackScript();
};