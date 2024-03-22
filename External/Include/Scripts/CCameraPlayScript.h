#pragma once
#include <Engine\CScript.h>
#include <Engine/CGameObject.h>

class CCameraPlayScript :
    public CScript
{
private:
    static CGameObject*      m_Target;

    float                    m_CamSpeed;

public:
    virtual void tick();
    static void SetTarget(CGameObject* _target) { m_Target = _target; }

public:
    void MoveOrthographic();
    void MovePerspective();
    virtual void SaveToFile(FILE* _File) override {};
    virtual void LoadFromFile(FILE* _File) override {};
    CLONE(CCameraPlayScript);

public:
    CCameraPlayScript();
    ~CCameraPlayScript();
};

