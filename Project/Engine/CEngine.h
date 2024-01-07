#pragma once



class CEngine
	: public CSingleton<CEngine>
{
	SINGLE(CEngine);

private:
	HWND m_hMainWnd;
	Vec2 m_vResolution;
	
public:
	HWND GetMainWind() { return m_hMainWnd; }

private:
	void DebugFunctionCheck();

public:
	int init(HWND _hWnd, Vec2 _vResolution);
	void Progress();

};

