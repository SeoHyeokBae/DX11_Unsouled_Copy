#pragma once
#include <Engine/singleton.h>

class CImGuiMgr :
    public CSingleton<CImGuiMgr>
{
    SINGLE(CImGuiMgr);

public: 
    void init(HWND _hMainWnd, ComPtr<ID3D11Device> _Device
        , ComPtr<ID3D11DeviceContext> _Context);
    void progress();

private:
    void tick();
    void render();

};

