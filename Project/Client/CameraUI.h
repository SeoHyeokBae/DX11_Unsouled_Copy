#pragma once
#include "ComponentUI.h"
class CameraUI :
    public ComponentUI
{
private:
    vector<string>    m_strVec;

public:
    void SelectType(const string _str);
    void SelectLayer(const string _str);

public:
    virtual void render_update();

public:
    CameraUI();
    ~CameraUI();
};

