#pragma once
#include "UI.h"

#include <Engine/CGameObject.h>

class TransformUI :
    public UI
{

private:
    CGameObject* m_TargetObject;

public:
    void SetTargetObject(CGameObject* _Target)
    {
        m_TargetObject = _Target;
    }

public:
    virtual void render_update() override;

public:
    TransformUI();
    ~TransformUI();
};

