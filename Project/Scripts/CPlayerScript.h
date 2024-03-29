#pragma once
#include <Engine/CScript.h>

enum class eDIR
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE,
};

class CPlayerScript :
    public CScript
{
private:
    Ptr<CPrefab>    m_Missile;
    float           m_Speed;
    eDIR            m_Dir;

public:
    eDIR GetDir() { return m_Dir; }

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

