#pragma once
#include "ComponentUI.h"

class Light2DUI :
    public ComponentUI
{

public:
    virtual void render_update() override;


private:
    void SelectDir(const string _str);

public:
    Light2DUI();
    ~Light2DUI();
};

