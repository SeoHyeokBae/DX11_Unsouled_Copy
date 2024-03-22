#include "pch.h"

#include "CTaskMgr.h"
#include "CRenderMgr.h"


void GamePlayStatic::SpawnGameObject(CGameObject* _Target, int _LayerIdx)
{
	tTask task = {};
	task.Type = TASK_TYPE::CREATE_OBJECT;
	task.Param_1 = (DWORD_PTR)_LayerIdx;
	task.Param_2 = (DWORD_PTR)_Target;
	CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::DestroyGameObject(CGameObject* _Target)
{
	tTask task = {};
	task.Type = TASK_TYPE::DELETE_OBJECT;
	task.Param_1 = (DWORD_PTR)_Target;
	CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::AddAsset(CAsset* _Asset)
{
	tTask task = {};
	task.Type = TASK_TYPE::ADD_ASSET;
	task.Param_1 = (DWORD_PTR)_Asset;
	CTaskMgr::GetInst()->AddTask(task);
}

// 행렬로 DebugRect 정보전달
void GamePlayStatic::DrawDebugRect(const Matrix& _WorldMat, Vec3 _Color, bool _bDepthTest, float _Duration)
{
	tDebugShapeInfo info = {};
	info.eShape = DEBUG_SHAPE::RECT;
	info.matWorld = _WorldMat;
	info.vColor = _Color;
	info.bDepthTest = _bDepthTest;
	info.fDuration = _Duration;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::Play2DSound(const wstring& _SoundPath, int _Loop, float _Volume, bool _Overlap)
{
	Ptr<CSound> pSound = CAssetMgr::GetInst()->Load<CSound>(_SoundPath, _SoundPath);

	if (nullptr != pSound)
	{
		pSound->Play(_Loop, _Volume, _Overlap);
	}
}

void GamePlayStatic::Play2DBGM(const wstring& _SoundPath, float _Volume)
{
	static Ptr<CSound> CurBGM = nullptr;

	Ptr<CSound> pSound = CAssetMgr::GetInst()->Load<CSound>(_SoundPath, _SoundPath);

	if (nullptr != pSound)
	{
		if (nullptr != CurBGM)
		{
			CurBGM->Stop();
		}

		pSound->Play(0, _Volume);
		CurBGM = pSound;
	}
}

// 하나하나 DebugRect 정보전달
void GamePlayStatic::DrawDebugRect(Vec3 _vWorldPos, Vec3 _vWorldScale, Vec3 _vWorldRot, Vec3 _Color, bool _bDepthTest, float _Duration)
{
	tDebugShapeInfo info = {};
	info.eShape = DEBUG_SHAPE::RECT;

	info.vWorldPos = _vWorldPos;
	info.vWorldScale = _vWorldScale;
	info.vWorldRot = _vWorldRot;

	info.matWorld = XMMatrixScaling(info.vWorldScale.x, info.vWorldScale.y, info.vWorldScale.z)
		* XMMatrixRotationX(info.vWorldRot.x) * XMMatrixRotationY(info.vWorldRot.y)
		* XMMatrixRotationZ(info.vWorldRot.z) * XMMatrixTranslation(info.vWorldPos.x, info.vWorldPos.y, info.vWorldPos.z);

	info.vColor = _Color;
	info.bDepthTest = _bDepthTest;
	info.fDuration = _Duration;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::DrawDebugCircle(Vec3 _vWorldPos, float _fRadius, Vec3 _Color, bool _bDepthTest, float _Duration)
{
	tDebugShapeInfo info = {};
	info.eShape = DEBUG_SHAPE::CIRCLE;

	info.vWorldPos = _vWorldPos;
	info.vWorldScale = Vec3(_fRadius * 2 , _fRadius * 2, 1.f);
	info.vWorldRot = Vec3(0.f, 0.f, 0.f);

	info.matWorld = XMMatrixScaling(info.vWorldScale.x, info.vWorldScale.y, info.vWorldScale.z)
		* XMMatrixRotationX(info.vWorldRot.x) * XMMatrixRotationY(info.vWorldRot.y)
		* XMMatrixRotationZ(info.vWorldRot.z) * XMMatrixTranslation(info.vWorldPos.x, info.vWorldPos.y, info.vWorldPos.z);

	info.vColor = _Color;
	info.bDepthTest = _bDepthTest;
	info.fDuration = _Duration;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::DrawDebugCross(Vec3 _vWorldPos, float _fScale, Vec3 _Color, bool _bDepthTest, float _Duration)
{
	tDebugShapeInfo info = {};
	info.eShape = DEBUG_SHAPE::CROSS;

	info.vWorldPos = _vWorldPos;
	info.vWorldScale = Vec3(_fScale, _fScale, _fScale);
	info.vWorldRot = Vec3(0.f, 0.f, 0.f);

	info.matWorld = XMMatrixScaling(info.vWorldScale.x, info.vWorldScale.y, info.vWorldScale.z)
		* XMMatrixRotationX(info.vWorldRot.x) * XMMatrixRotationY(info.vWorldRot.y)
		* XMMatrixRotationZ(info.vWorldRot.z) * XMMatrixTranslation(info.vWorldPos.x, info.vWorldPos.y, info.vWorldPos.z);

	info.vColor = _Color;
	info.bDepthTest = _bDepthTest;
	info.fDuration = _Duration;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void Vector3::ToDegree()
{
	x = (x / XM_PI) * 180.f;
	y = (y / XM_PI) * 180.f;
	z = (z / XM_PI) * 180.f;
}

void Vector3::ToRadian()
{
	x = (x / 180.f) * XM_PI;
	y = (y / 180.f) * XM_PI;
	z = (z / 180.f) * XM_PI;
}

string ToString(const wstring& _str)
{
	return string(_str.begin(), _str.end());
}

wstring ToWString(const string& _str)
{
	return wstring(_str.begin(), _str.end());
}

void CharToWChar(const char* pstrSrc, wchar_t pwstrDest[])
{
	int nLen = (int)strlen(pstrSrc) + 1;
	mbstowcs(pwstrDest, pstrSrc, nLen);
}

void SaveWString(const wstring& _str, FILE* _File)
{
	size_t len = _str.length();
	fwrite(&len, sizeof(size_t), 1, _File);
	fwrite(_str.c_str(), sizeof(wchar_t), _str.length(), _File);
}

void LoadWString(wstring& _str, FILE* _File)
{
	size_t len = 0;
	wchar_t szBuff[256] = {};
	fread(&len, sizeof(size_t), 1, _File);
	fread(szBuff, sizeof(wchar_t), len, _File);
	_str = szBuff;
}
               
#include <shobjidl_core.h>
void OpenFileDialog(vector<wstring>& _FilesName)
{
    IFileOpenDialog* pFileDialog;
    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileDialog));
    if (FAILED(hr))
    {
        std::cerr << "Failed to create FileOpenDialog instance" << std::endl;
        return;
    }

    // 다중 선택 가능 설정
    DWORD dwOptions;
    hr = pFileDialog->GetOptions(&dwOptions);
    if (SUCCEEDED(hr))
    {
        hr = pFileDialog->SetOptions(dwOptions | FOS_ALLOWMULTISELECT);
    }

    if (SUCCEEDED(hr))
    {
        // 원하는 초기 디렉토리 경로를 여기에 설정
        wstring MetaPath = CPathMgr::GetContentPath();
        MetaPath += L"meta";

        PWSTR initialDir = (PWSTR)(MetaPath.c_str());

        IShellItem* pInitialDirItem;

        hr = SHCreateItemFromParsingName(initialDir, NULL, IID_IShellItem, reinterpret_cast<void**>(&pInitialDirItem));

        // 파일 대화 상자 인터페이스에 초기 디렉토리 설정
        hr = pFileDialog->SetFolder(pInitialDirItem);
    }

    // 파일 필터 설정
    COMDLG_FILTERSPEC fileTypes[] = { {L"All Files", L"*.*"}, {L"Text Files", L"*.txt"}, {L"FBX Files", L"*.fbx"} };
    hr = pFileDialog->SetFileTypes(ARRAYSIZE(fileTypes), fileTypes);
    if (FAILED(hr))
    {
        std::cerr << "Failed to set file types" << std::endl;
        pFileDialog->Release();
        return;
    }

    // 대화 상자 열기
    hr = pFileDialog->Show(NULL);
    if (FAILED(hr))
    {
        std::cerr << "Failed to open FileOpenDialog" << std::endl;
        pFileDialog->Release();
        return;
    }

    // 선택된 파일 목록 가져오기
    IShellItemArray* pItems;
    hr = pFileDialog->GetResults(&pItems);
    if (FAILED(hr))
    {
        std::cerr << "Failed to get selected items" << std::endl;
        pFileDialog->Release();
        return;
    }

    // 선택된 파일들의 경로 가져오기
    DWORD itemCount;
    hr = pItems->GetCount(&itemCount);
    if (SUCCEEDED(hr))
    {
        for (DWORD i = 0; i < itemCount; ++i)
        {
            IShellItem* pItem;
            hr = pItems->GetItemAt(i, &pItem);
            if (SUCCEEDED(hr))
            {
                PWSTR pszFilePath;
                hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
                if (SUCCEEDED(hr))
                {
                    _FilesName.push_back(pszFilePath);
                    CoTaskMemFree(pszFilePath);
                }
                pItem->Release();
            }
        }
    }
    pItems->Release();
    pFileDialog->Release();
}

Vec2 LoadMeta(const wstring& _strMetaRelativePath)
{
    Vec2 retVec = { -1, -1 };
    FILE* pFile = nullptr;

    _wfopen_s(&pFile, (_strMetaRelativePath).c_str(), L"r");

    if (nullptr == pFile)
    {
        MessageBoxA(nullptr, "Meta 파일이 존재하지 않습니다.", "Meta File No Exist!", MB_OK);
        return {};
    }

    // Animation 이름 로드

    while (true)
    {
        wchar_t szRead[256] = {};
        float tmpfloat = -1.f;

        if (EOF == fwscanf_s(pFile, L"%s", szRead, 256))
        {
            break;
        }

        if (!wcscmp(szRead, L"m_Offset:"))
        {
            while (true)
            {
                fwscanf_s(pFile, L"%s", szRead, 256);

                if (!wcscmp(szRead, L"{x:"))
                {
                    fwscanf_s(pFile, L"%f", &retVec.x);
                    retVec.x *= -1;
                }
                if (!wcscmp(szRead, L"y:"))
                {
                    fwscanf_s(pFile, L"%s", szRead, 256);

                    int length = (int)wcslen(szRead);

                    // 끝에 한글자 잘라야됨 1.24} 라고 되어있음
                    if (length > 0)
                    {
                        szRead[length - 1] = '\0';
                    }

                    wchar_t* end;
                    float tmp = wcstof(szRead, &end);

                    if (*end == L'\0')
                    {
                        retVec.y = tmp;
                    }

                    return retVec;
                }

                // 탈출 조건
                if (!wcscmp(szRead, L"m_Border:"))
                {
                    break;
                }
            }
        }
        // 탈출 조건
        if (!wcscmp(szRead, L"m_Border:"))
            break;
    }

    fclose(pFile);

    return retVec;
}
