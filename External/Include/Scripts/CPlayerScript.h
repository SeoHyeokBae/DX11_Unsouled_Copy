#pragma once
#include <Engine/CScript.h>

class CGameObject;

class CPlayerScript :
    public CScript
{
private:
    CGameObject*    m_HitBox;
    CGameObject*    m_TopBodyCol;
    CGameObject*    m_AttCol;
    wstring         m_CurState;

    float           m_Speed;
    float           m_AftTime;
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

