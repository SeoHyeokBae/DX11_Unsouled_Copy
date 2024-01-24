#pragma once
#include "ComponentUI.h"
class Animator2DUI :
    public ComponentUI
{
public:
    virtual void render_update() override;

public:
    void AnimSelect(DWORD_PTR _ptr);

public:
    Animator2DUI();
    ~Animator2DUI();
};

