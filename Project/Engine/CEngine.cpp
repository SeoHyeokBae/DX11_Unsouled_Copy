#include "pch.h"
#include "CEngine.h"

#include "CDevice.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CPathMgr.h"
#include "CAssetMgr.h"
#include "CLevelMgr.h"
#include "CTaskMgr.h"
#include "CGC.h"
#include "CRenderMgr.h"
#include "CCollisionMgr.h"

CEngine::CEngine()
	: m_hMainWnd(nullptr)
	, m_vResolution()
{

}

CEngine::~CEngine()
{
}

int CEngine::init(HWND _hWnd, Vec2 _vResolution)
{
	m_hMainWnd = _hWnd;
	m_vResolution = _vResolution;

	RECT rt = { 0,0,(int)m_vResolution.x,(int)m_vResolution.y };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(m_hMainWnd, nullptr, 10, 10, rt.right - rt.left, rt.bottom - rt.top,0);

	if (FAILED(CDevice::GetInst()->init(m_hMainWnd, m_vResolution)))
	{
		MessageBox(nullptr, L"CDevice 초기화 실패", L"초기화 실패", MB_OK);
		return E_FAIL;
	}
	// Manager 초기화
	CPathMgr::init();
	CTimeMgr::GetInst()->init();
	CKeyMgr::GetInst()->init();
	CAssetMgr::GetInst()->init();
	CRenderMgr::GetInst()->init();
	CLevelMgr::GetInst()->init();


	return S_OK;
}

void CEngine::Progress()
{	
	// Manager Update
	CTimeMgr::GetInst()->tick();
	CKeyMgr::GetInst()->tick();

	CLevelMgr::GetInst()->tick();
	CCollisionMgr::GetInst()->tick();
	CRenderMgr::GetInst()->tick();

	// GC
	CGC::GetInst()->tick();

	// Task
	CTaskMgr::GetInst()->tick();

	DebugFunctionCheck();
}

void CEngine::DebugFunctionCheck()
{
	if (KEY_TAP(KEY::O))
	{
		CRenderMgr::GetInst()->IsDebugPosition() ? CRenderMgr::GetInst()->SetDebugPosition(false) : CRenderMgr::GetInst()->SetDebugPosition(true);
	}
}