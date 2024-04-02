#pragma once
#include <Engine\CScript.h>

class CPlayerHitBox :
    public CScript
{
private:
    CGameObject*    m_ColObj;

public:

public:
    virtual void tick() override;
    virtual void begin() override;
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider);

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CPlayerHitBox);
public:
    CPlayerHitBox();
    ~CPlayerHitBox();
};

