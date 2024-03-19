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
#include <Scripts/CMonsterScript.h>

#include <Engine/CAssetMgr.h>
#include <Engine/CPrefab.h>
#include <Engine/CFSM.h>

#include "CIdleState.h"
#include "CTraceState.h"

void CCreateTempLevel::Init()
{
	// Missile Prefab ����
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

	// �ӽ� FSM ��ü ���� �ϳ� �����ϱ�
	Ptr<CFSM>	pFSM = new CFSM(true);

	pFSM->AddState(L"IdleState", new CIdleState);
	pFSM->AddState(L"TraceState", new CTraceState);

	CAssetMgr::GetInst()->AddAsset<CFSM>(L"NormalMonsterFSM", pFSM.Get());
	
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

	Ptr<CTexture> pAltasTex = CAssetMgr::GetInst()->Load<CTexture>(L"testAtlas", L"texture\\player_tilesheet.png");
	pAltasTex = CAssetMgr::GetInst()->Load<CTexture>(L"AnimAtlasTex", L"texture\\link.png");
	
	// �ʱ� ���� �����ϱ�
	CLevel* pTempLevel = new CLevel;
	pTempLevel->GetLayer(0)->SetName(L"Default");
	pTempLevel->GetLayer(1)->SetName(L"Background");
	pTempLevel->GetLayer(2)->SetName(L"Tile");
	pTempLevel->GetLayer(3)->SetName(L"Player");
	pTempLevel->GetLayer(4)->SetName(L"Monster");
	pTempLevel->GetLayer(5)->SetName(L"Light");
	pTempLevel->GetLayer(31)->SetName(L"UI");


	// ComputeShader �׽�Ʈ
	// ����� �ؽ��� ����
	Ptr<CTexture> pTestTex = CAssetMgr::GetInst()->CreateTexture(L"TestTex"
		, 1024, 1024, DXGI_FORMAT_R8G8B8A8_UNORM
		, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS);

	Ptr<CSetColorShader> pCS = (CSetColorShader*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"SetColorShader").Get();
	pCS->SetColor(Vec3(1.f, 0.f, 0.f));
	pCS->SetTargetTexture(pTestTex);
	pCS->Execute();

	// Camera Object ����
	// Main Camera Object ����
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

	// UIī�޶� ����
	pCamObj = new CGameObject;
	pCamObj->SetName(L"UICamera");
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);

	pCamObj->Transform()->SetRelativePos(Vec3(0.5f, 0.f, 0.f));
	pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

	pCamObj->Camera()->SetCameraPriority(1);
	pCamObj->Camera()->LayerCheck(31, true);

	pTempLevel->AddObject(pCamObj, 0);


	// ���� �߰�
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

	// Backgruond Object ����
	CGameObject* pObj = new CGameObject;
	pObj->SetName(L"Background");

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(new CBackgroundScript);

	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 1500.f));
	pObj->Transform()->SetRelativeScale(Vec3(203.f*2.5, 243.f*2.5, 1.f));

	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BackgroundMtrl"));

	Ptr<CTexture> pTex = CAssetMgr::GetInst()->Load<CTexture>(L"BackgroundTex", L"texture\\Background.png");
	pObj->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);

	pTempLevel->AddObject(pObj, L"Background", false);

	// Player Object ����
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
	pObj->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"PlayerTexture", L"texture\\port.png"));

	pObj->Animator2D()->Create(L"IDLE_UP", pAltasTex, Vec2(0.f, 260.f), Vec2(120.f, 130.f), Vec2(0.f, 0.f), Vec2(200.f, 200.f), 1, 10);
	pObj->Animator2D()->Create(L"IDLE_DOWN", pAltasTex, Vec2(0.f, 0.f), Vec2(120.f, 130.f), Vec2(0.f, 0.f), Vec2(200.f, 200.f), 3, 10);
	pObj->Animator2D()->Create(L"IDLE_LEFT", pAltasTex, Vec2(0.f, 130.f), Vec2(120.f, 130.f), Vec2(0.f, 0.f), Vec2(200.f, 200.f), 3, 10);
	pObj->Animator2D()->Create(L"IDLE_RIGHT", pAltasTex, Vec2(0.f, 390.f), Vec2(120.f, 130.f), Vec2(0.f, 0.f), Vec2(200.f, 200.f), 3, 10);

	pObj->Animator2D()->Create(L"MOVE_UP", pAltasTex, Vec2(0.f, 780.f), Vec2(120.f, 130.f), Vec2(0.f, 0.f), Vec2(200.f, 200.f), 10, 20);
	pObj->Animator2D()->Create(L"MOVE_DOWN", pAltasTex, Vec2(0.f, 520.f), Vec2(120.f, 130.f), Vec2(0.f, 0.f), Vec2(200.f, 200.f), 10, 20);
	pObj->Animator2D()->Create(L"MOVE_LEFT", pAltasTex, Vec2(0.f, 650.f), Vec2(120.f, 130.f), Vec2(0.f, 0.f), Vec2(200.f, 200.f), 10, 20);
	pObj->Animator2D()->Create(L"MOVE_RIGHT", pAltasTex, Vec2(0.f, 910.f), Vec2(120.f, 130.f), Vec2(0.f, 0.f), Vec2(200.f, 200.f), 10, 20);

	
	pTempLevel->AddObject(pObj, L"Player", false);
	CGameObject* pCObj = nullptr;
	pCObj = pObj->Clone();
	Vec3 vRot = pCObj->Transform()->GetRelativeRotation();
	vRot.z += XM_PI * 0.25f;
	vRot.x += XM_PI * 0.25f;
	pCObj->Transform()->SetRelativeRotation(vRot);
	pCObj->Transform()->SetRelativePos(Vec3(0.f,0.f,250.f));
	pObj->AddChild(pCObj);

	// Monster Object ����
	pObj = new CGameObject;
	pObj->SetName(L"Monster");

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(new CCollider2D);
	pObj->AddComponent(new CStateMachine);
	pObj->AddComponent(new CMonsterScript);
	pObj->AddComponent(new CAnimator2D);

	pObj->Transform()->SetRelativePos(Vec3(500.f, 0.f, 500.f));
	pObj->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));

	pObj->Collider2D()->SetAbsolute(true);
	pObj->Collider2D()->SetOffsetScale(Vec2(120.f, 120.f));
	pObj->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));

	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
	pObj->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Fighter.bmp", L"texture\\Fighter.bmp"));

	pObj->StateMachine()->SetFSM(CAssetMgr::GetInst()->FindAsset<CFSM>(L"NormalMonsterFSM"));

	pTempLevel->AddObject(pObj, L"Monster", false);

	// Particle Object
	CGameObject* pParticleObj = new CGameObject;
	pParticleObj->SetName(L"Particle");

	pParticleObj->AddComponent(new CTransform);
	pParticleObj->AddComponent(new CParticleSystem);

	pParticleObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 200.f));
	pTempLevel->AddObject(pParticleObj, L"Default", false);

	pParticleObj = pParticleObj->Clone();
	pParticleObj->Transform()->SetRelativePos(Vec3(-500.f, 0.f, 200.f));
	pTempLevel->AddObject(pParticleObj, L"Default", false);


	// UI object ����
	pObj = new CGameObject;
	pObj->SetName(L"UI");

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);

	pObj->Transform()->SetRelativePos(Vec3(-590, 310.f, 500.f));
	pObj->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 1.f));


	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));

	pTempLevel->AddObject(pObj, L"UI", false);


	// Camera LayerCheck �� �浹�������� LayerCheck �� m_CurLevel == nullptr �̹Ƿ� Idx��ȣ�� 

	// �浹 ����
	CCollisionMgr::GetInst()->LayerCheck(3, 4);
	CCollisionMgr::GetInst()->LayerCheck(4, 4);

	CLevelMgr::GetInst()->ChangeLevel(pTempLevel, LEVEL_STATE::STOP);

	CLevelSaveLoad::SaveLevel(pTempLevel, L"level\\temp.lv");
}
