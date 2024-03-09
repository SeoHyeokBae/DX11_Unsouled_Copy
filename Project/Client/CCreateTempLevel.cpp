#include "pch.h"
#include "CCreateTempLevel.h"

#include <Engine/CCollisionMgr.h>

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>

#include <Scripts/CPlayerScript.h>
#include <Scripts/CBackgroundScript.h>

#include <Engine/CMesh.h>
#include <Engine/CGraphicsShader.h>
#include <Engine/CTexture.h>
#include <Engine/CSetColorShader.h>

#include "CLevelSaveLoad.h"

#include <Scripts/CMissileScript.h>

#include <Engine/CAssetMgr.h>
#include <Engine/CPrefab.h>

void CCreateTempLevel::Init()
{
	// Missile Prefab 생성
	//CGameObject* pObj = nullptr;

	//pObj = new CGameObject;
	//pObj->SetName(L"Missile");
	//pObj->AddComponent(new CTransform);
	//pObj->AddComponent(new CMeshRender);
	//pObj->AddComponent(new CMissileScript);

	//pObj->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 1.f));

	//pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	//pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));

	//Ptr<CPrefab> pMissilePrefab = new CPrefab(pObj);
	//CAssetMgr::GetInst()->AddAsset<CPrefab>(L"MissilePrefab", pMissilePrefab.Get());


	//pMissilePrefab->Save(L"prefab\\missile.pref");
	
}

void CCreateTempLevel::CreateTempLevel()
{

	//Ptr<CMaterial> pBackgroudMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BackgroundMtrl");
	//Ptr<CMaterial> pStd2DMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl");

	//pBackgroudMtrl->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"BackgroundTex", L"texture\\Background.png"));
	//pStd2DMtrl->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"PlayerTexture", L"texture\\Fighter.bmp"));

	//CLevel* pLevel = CLevelSaveLoad::LoadLevel(L"level\\temp.lv");
	//CLevelMgr::GetInst()->ChangeLevel(pLevel, LEVEL_STATE::STOP);
	//return;

	Ptr<CTexture> pAltasTex = CAssetMgr::GetInst()->Load<CTexture>(L"penitent_running_anim", L"texture\\penitent_running_anim.png");
	pAltasTex = CAssetMgr::GetInst()->Load<CTexture>(L"AnimAtlasTex", L"texture\\link.png");
	
	// 초기 레벨 구성하기
	CLevel* pTempLevel = new CLevel;
	pTempLevel->GetLayer(0)->SetName(L"Default");
	pTempLevel->GetLayer(1)->SetName(L"Background");
	pTempLevel->GetLayer(2)->SetName(L"Tile");
	pTempLevel->GetLayer(3)->SetName(L"Player");
	pTempLevel->GetLayer(4)->SetName(L"Monster");
	pTempLevel->GetLayer(5)->SetName(L"Light");
	pTempLevel->GetLayer(31)->SetName(L"UI");


	// ComputeShader 테스트
	// 사용할 텍스쳐 생성
	Ptr<CTexture> pTestTex = CAssetMgr::GetInst()->CreateTexture(L"TestTex"
		, 1024, 1024, DXGI_FORMAT_R8G8B8A8_UNORM
		, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS);

	Ptr<CSetColorShader> pCS = (CSetColorShader*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"SetColorShader").Get();
	pCS->SetColor(Vec3(1.f, 0.f, 0.f));
	pCS->SetTargetTexture(pTestTex);
	pCS->Execute();

	tPixel* pPixel = pTestTex->GetPixels();
	tPixel pixel = pPixel[pTestTex->GetWidth() * 1 + 5];



	// Camera Object 생성
	// Main Camera Object 생성
	CGameObject* pCamObj = new CGameObject;
	pCamObj->SetName(L"MainCamera");
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);

	pCamObj->Transform()->SetRelativePos(Vec3(0.5f, 0.f, 0.f));
	pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

	pCamObj->Camera()->SetCameraPriority(0);
	pCamObj->Camera()->LayerCheckAll();
	pCamObj->Camera()->LayerCheck(31, false);

	pTempLevel->AddObject(pCamObj, 0);

	// UI카메라 생성
	pCamObj = new CGameObject;
	pCamObj->SetName(L"UICamera");
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);

	pCamObj->Transform()->SetRelativePos(Vec3(0.5f, 0.f, 0.f));
	pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

	pCamObj->Camera()->SetCameraPriority(1);
	pCamObj->Camera()->LayerCheck(31, true);

	pTempLevel->AddObject(pCamObj, 0);


	// 광원 추가
	CGameObject* pLight = new CGameObject;
	pLight->SetName(L"Directional Light");
	pLight->AddComponent(new CTransform);
	//pLight->AddComponent(new CMeshRender);
	pLight->AddComponent(new CLight2D);

	pLight->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLight->Light2D()->SetAmbient(Vec3(0.3f, 0.3f, 0.3f));
	//pLight->Light2D()->SetRadius(400.f);
	//pLight->Light2D()->SetAngle(60.f);
	pTempLevel->AddObject(pLight, L"Light");

	// Backgruond Object 생성
	CGameObject* pObj = new CGameObject;
	pObj->SetName(L"Background");

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(new CBackgroundScript);

	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 600.f));
	pObj->Transform()->SetRelativeScale(Vec3(1280.f, 720.f, 1.f));

	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BackgroundMtrl"));

	Ptr<CTexture> pTex = CAssetMgr::GetInst()->Load<CTexture>(L"BackgroundTex", L"texture\\Background.png");
	pObj->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);

	pTempLevel->AddObject(pObj, L"Background", false);

	// Player Object 생성
	pObj = new CGameObject;
	pObj->SetName(L"Player");

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(new CCollider2D);
	pObj->AddComponent(new CAnimator2D);
	pObj->AddComponent(new CPlayerScript);
	//pObj->Animator2D()->Create(L"IDLE_LEFT", pAltasTex, Vec2(0.f, 130.f), Vec2(120.f, 130.f), Vec2(0.f, 0.f), Vec2(200.f, 200.f), 3, 10);


	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 500.f));
	pObj->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));

	pObj->Collider2D()->SetAbsolute(true);
	pObj->Collider2D()->SetOffsetScale(Vec2(100.f, 100.f));
	pObj->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));

	pObj->AddComponent(new CLight2D);
	pObj->Light2D()->SetLightType(LIGHT_TYPE::POINT);
	pObj->Light2D()->SetLightColor(Vec3(1.f, 1.f, 1.f));
	pObj->Light2D()->SetRadius(200.f);
	//pObj->Light2D()->SetAngle(60.f);


	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
	pObj->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"PlayerTexture", L"texture\\Fighter.bmp"));

	pTempLevel->AddObject(pObj, L"Player", false);

	pObj = pObj->Clone();
	pObj->Transform()->SetRelativePos(Vec3(-500.f, 0.f, 500.f));
	//m_CurLevel->AddObject(pObj, L"Player", false);

	// Particle Object
	CGameObject* pParticleObj = new CGameObject;
	pParticleObj->SetName(L"Particle");

	pParticleObj->AddComponent(new CTransform);
	pParticleObj->AddComponent(new CParticleSystem);

	pParticleObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 200.f));
	pTempLevel->AddObject(pParticleObj, L"Default", false);

	pParticleObj = pParticleObj->Clone();
	pParticleObj->Transform()->SetRelativePos(Vec3(-500.f, 0.f, 200.f));
	//m_CurLevel->AddObject(pParticleObj, L"Default", false);

	pObj->AddChild(pParticleObj);
	pTempLevel->AddObject(pObj, L"Default", false);

	CGameObject* pCloneObj = pObj->Clone();
	pTempLevel->AddObject(pCloneObj, L"Default", false);

	// UI object 생성
	pObj = new CGameObject;
	pObj->SetName(L"UI");

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);

	pObj->Transform()->SetRelativePos(Vec3(-590, 310.f, 500.f));
	pObj->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 1.f));


	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));

	pTempLevel->AddObject(pObj, L"UI", false);


	// Camera LayerCheck 와 충돌설정에서 LayerCheck 시 m_CurLevel == nullptr 이므로 Idx번호로 

	// 충돌 설정
	CCollisionMgr::GetInst()->LayerCheck(3, 4);
	CCollisionMgr::GetInst()->LayerCheck(4, 4);

	CLevelMgr::GetInst()->ChangeLevel(pTempLevel, LEVEL_STATE::PLAY);

	CLevelSaveLoad::SaveLevel(pTempLevel, L"level\\temp.lv");
}
