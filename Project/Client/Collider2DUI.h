#pragma once
#include "ComponentUI.h"

class Collider2DUI :
    public ComponentUI
{
    virtual void render_update() override;

private:
    void SelectType(const string _str);

public:
    Collider2DUI();
    ~Collider2DUI();
};

