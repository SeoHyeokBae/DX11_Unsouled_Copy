#pragma once
#include "UI.h"
class MenuUI :
    public UI
{

public:
    virtual void render();
    virtual void render_update();

public:
    MenuUI();
    ~MenuUI();
};

