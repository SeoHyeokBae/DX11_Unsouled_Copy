#pragma once
#include "UI.h"



class ListUI :
    public UI
{
private:
    vector<string>      m_vecStr;
    string              m_strDBClicked;

    CALL_BACK_1         m_CallBackFunc;

public:
    void AddString(const string& _str) { m_vecStr.push_back(_str); }
    void AddString(const vector<string>& _vecStr)
    {
        m_vecStr.insert(m_vecStr.end(), _vecStr.begin(), _vecStr.end());
    }

    void SetDbClickCallBack(CALL_BACK_1 _CallBack)
    {
        m_CallBackFunc = _CallBack;
    }

    string GetDoubleClicked() { return m_strDBClicked; }



public:
    virtual void render_update() override;
    virtual void Deactivate() override;



public:
    ListUI();
    ~ListUI();
};

