#pragma once
#include "UI.h"

// 현재 레벨에 있는 모든 오브젝트들 나열

class CLevel;

class Outliner :
    public UI
{
private:
    vector<string>      m_vecStr;
    string              m_strDBClicked;
    CLevel*             m_CurLevel;


public:
    void AddString(const string& _str) { m_vecStr.push_back(_str); }
    void AddString(const vector<string>& _vecStr)
    {
        m_vecStr.insert(m_vecStr.end(), _vecStr.begin(), _vecStr.end());
    }

    void ObjectSelect(const string& _ptr);

public:
    virtual void render_update() override;

public:
    Outliner();
    ~Outliner();
};

