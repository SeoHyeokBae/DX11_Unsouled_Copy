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

#include <Scripts/CCameraPlayScript.h>
#include <Scripts/CMissileScript.h>
#include <Scripts/CMonsterScript.h>
#include <Scripts/CShadowScript.h>
#include <Scripts/CColTileScript.h>
#include <Scripts/CAfterImageScript.h>
#include <Scripts/CZSortScript.h>
#include <Scripts/CChainSystemScript.h>
#include <Scripts/CPlayerHitBox.h>
#include <Scripts/CEffectScript.h>
#include <Scripts/CNormalObjScript.h>
#include <Scripts/CZombieScript.h>
#include <Scripts/CBossNiugScript.h>
#include <Scripts/CPlayerAttackScript.h>
#include <Scripts/CBloodScript.h>

#include <Scripts/CHitColliderScript.h>


#include <Engine/CAssetMgr.h>
#include <Engine/CPrefab.h>
#include <Engine/CFSM.h>

#include "CIdleState.h"
#include "CTraceState.h"


#include "CStaminaOutState.h"
#include "CRunningState.h"
#include "CStandState.h"
#include "CAbsorbState.h"
#include "CAttackState.h"
#include "CDashState.h"
#include "CDashAttState.h"
#include "CPlayerBlockState.h"


#include "CNiug_RunningState.h"
#include "CNiug_BasicAttState.h"
#include "CNiug_BallistaSoulderState.h"
#include "CNiug_BallistaReadyState.h"
#include "CNiug_RageOfIsno.h"



void CCreateTempLevel::Init()
{
	// 프리펩 저장 하드코딩
	//CGameObject* pObj2 = nullptr;

	//pObj2 = new CGameObject;
	//pObj2->SetName(L"Blood");
	//pObj2->AddComponent(new CTransform);
	//pObj2->AddComponent(new CMeshRender);
	//pObj2->AddComponent(new CAnimator2D);
	//pObj2->AddComponent(new CMovement);
	//pObj2->AddComponent(new CZSortScript);
	//pObj2->AddComponent(new CBloodScript);
	//pObj2->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	//pObj2->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BloodMarkMtrl"));
	//pObj2->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));
	//pObj2->Transform()->SetRelativePos(Vec3(0.f, 10.f, 0.f));
	//Ptr<CPrefab> pPrefab = new CPrefab(pObj2, false);
	////CAssetMgr::GetInst()->AddAsset<CPrefab>(L"Niug_HitCollider", pPrefab.Get());
	//pPrefab->Save(L"prefab\\Blood.pref");
	


	// 임시 FSM 객체 에셋 하나 생성하기
	// Zombie
	Ptr<CFSM>	pFSM = new CFSM(true);
	pFSM->AddState(L"IdleState", new CIdleState);
	pFSM->AddState(L"TraceState", new CTraceState);
	CAssetMgr::GetInst()->AddAsset<CFSM>(L"NormalMonsterFSM", pFSM.Get());

	// Boss Niug
	pFSM = new CFSM(true);
	pFSM->AddState(L"RunningState", new CNiug_RunningState);
	pFSM->AddState(L"BasicAttState", new CNiug_BasicAttState);
	pFSM->AddState(L"RageOfIsnoState", new CNiug_RageOfIsno);
	pFSM->AddState(L"BallistaReadyState", new CNiug_BallistaReadyState);
	pFSM->AddState(L"BallistaSoulderState", new CNiug_BallistaSoulderState);
	CAssetMgr::GetInst()->AddAsset<CFSM>(L"Boss_NiugFSM", pFSM.Get());

	// Player
	pFSM = new CFSM(true);
	pFSM->AddState(L"StaminaOutState", new CStaminaOutState);
	pFSM->AddState(L"RunningState", new CRunningState);
	pFSM->AddState(L"StandState", new CStandState);
	pFSM->AddState(L"AbsorbState", new CAbsorbState);
	pFSM->AddState(L"AttackState", new CAttackState);
	pFSM->AddState(L"DashState", new CDashState);
	pFSM->AddState(L"DashAttState", new CDashAttState);
	pFSM->AddState(L"BlockState", new CPlayerBlockState);
	CAssetMgr::GetInst()->AddAsset<CFSM>(L"PlayerFSM", pFSM.Get());
}

void CCreateTempLevel::CreateTempLevel()
{
	// 초기 레벨 구성하기
	CLevel* pTempLevel = new CLevel;
	pTempLevel->GetLayer(0)->SetName(L"Default");
	pTempLevel->GetLayer(1)->SetName(L"FrontWall");
	pTempLevel->GetLayer(2)->SetName(L"Player");
	pTempLevel->GetLayer(3)->SetName(L"Monster");
	pTempLevel->GetLayer(4)->SetName(L"Light");
	pTempLevel->GetLayer(5)->SetName(L"Shadow");

	pTempLevel->GetLayer(11)->SetName(L"Tile_Property");
	pTempLevel->GetLayer(12)->SetName(L"Tile_Collider");
	pTempLevel->GetLayer(10)->SetName(L"Background");

	pTempLevel->GetLayer(20)->SetName(L"PlayerHitCollider");
	pTempLevel->GetLayer(21)->SetName(L"PlayerHitBoxCollider");
	pTempLevel->GetLayer(22)->SetName(L"PlayerBodyCollider");
	pTempLevel->GetLayer(25)->SetName(L"MonsterHitCollider");
	pTempLevel->GetLayer(26)->SetName(L"MonsterLockOn");
	pTempLevel->GetLayer(31)->SetName(L"UI");


	// ComputeShader 테스트
	// 사용할 텍스쳐 생성
	//Ptr<CTexture> pTestTex = CAssetMgr::GetInst()->CreateTexture(L"TestTex"
	//	, 1024, 1024, DXGI_FORMAT_R8G8B8A8_UNORM
	//	, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS);

	//Ptr<CSetColorShader> pCS = (CSetColorShader*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"SetColorShader").Get();
	//pCS->SetColor(Vec3(1.f, 0.f, 0.f));
	//pCS->SetTargetTexture(pTestTex);
	//pCS->Execute();

	// Camera Object 생성
	// Main Camera Object 생성
	CGameObject* pCamObj = new CGameObject;
	pCamObj->SetName(L"MainCamera");
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	pCamObj->AddComponent(new CCameraPlayScript);

	pCamObj->Transform()->SetRelativePos(Vec3(-70.f, 100.f, 0.f));
	pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

	pCamObj->Camera()->SetCameraPriority(0);
	//pCamObj->Camera()->SetScale(0.3f);
	pCamObj->Camera()->SetScale(0.5f);

	pCamObj->Camera()->LayerCheckAll();
	pCamObj->Camera()->LayerCheck(31, false);

	pTempLevel->AddObject(pCamObj, 0);

	// UI카메라 생성
	CGameObject* pCamUIObj = new CGameObject;
	pCamUIObj->SetName(L"UICamera");
	pCamUIObj->AddComponent(new CTransform);
	pCamUIObj->AddComponent(new CCamera);

	pCamUIObj->Transform()->SetRelativePos(Vec3(0.5f, 0.f, 0.f));
	pCamUIObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

	pCamUIObj->Camera()->SetCameraPriority(1);
	pCamUIObj->Camera()->LayerCheck(31, true);

	pTempLevel->AddObject(pCamUIObj, 0);


	// 광원 추가
	CGameObject* pLight = new CGameObject;
	pLight->SetName(L"Directional Light");
	pLight->AddComponent(new CTransform);
	//pLight->AddComponent(new CMeshRender);
	pLight->AddComponent(new CLight2D);

	pLight->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLight->Light2D()->SetAmbient(Vec3(0.7f, 0.7f, 0.7f));
	//pLight->Light2D()->SetRadius(400.f);
	//pLight->Light2D()->SetAngle(60.f);
	pTempLevel->AddObject(pLight, L"Light");

	// Backgruond Object 생성
	CGameObject* pObj = new CGameObject;
	//pObj->SetName(L"Background");

	//pObj->AddComponent(new CTransform);
	//pObj->AddComponent(new CMeshRender);
	//pObj->AddComponent(new CBackgroundScript);

	//pObj->Transform()->SetRelativePos(Vec3(111.f, 1321.f, 1500.f));
	//pObj->Transform()->SetRelativeScale(Vec3(832.f, 2960.f, 1.f));

	//pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	//pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BackgroundMtrl"));

	//Ptr<CTexture> pTex = CAssetMgr::GetInst()->Load<CTexture>(L"BackgroundTex", L"texture\\intro_floor.png");
	//pObj->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);

	//pTempLevel->AddObject(pObj, L"Background", false);


	// Player Object 생성
	//pObj = new CGameObject;
	pObj->SetName(L"Player");

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(new CCollider2D);
	pObj->AddComponent(new CAnimator2D);
	pObj->AddComponent(new CShadowScript);
	pObj->AddComponent(new CMovement);
	pObj->AddComponent(new CStateMachine);
	pObj->AddComponent(new CAfterImageScript);
	pObj->AddComponent(new CPlayerAttackScript);
	pObj->AddComponent(new CZSortScript);
	pObj->AddComponent(new CChainSystemScript);
	pObj->AddComponent(new CEffectScript);

	//pObj->Animator2D()->Create(L"IDLE_LEFT", pAltasTex, Vec2(0.f, 130.f), Vec2(120.f, 130.f), Vec2(0.f, 0.f), Vec2(200.f, 200.f), 3, 10);

	pObj->Transform()->SetRelativePos(Vec3(0.f, 50.f, 0.f));
	pObj->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));

	pObj->Collider2D()->SetAbsolute(true);
	pObj->Collider2D()->SetOffsetScale(Vec2(12.f, 3.f));
	pObj->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
	//pObj->Collider2D()->SetVisible(true);

	pObj->Movement()->SetMass(1.f);
	pObj->Movement()->SetInitSpeed(125.f);
	pObj->Movement()->SetMaxSpeed(125.f);
	pObj->Movement()->SetFrictionScale(1000.f);

	pObj->Movement()->SetGround(true);

	//pObj->AddComponent(new CLight2D);
	//pObj->Light2D()->SetLightType(LIGHT_TYPE::POINT);
	//pObj->Light2D()->SetLightColor(Vec3(1.f, 1.f, 1.f));
	//pObj->Light2D()->SetRadius(200.f);
	//pObj->Light2D()->SetAngle(60.f);

	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
	pObj->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"PlayerTexture", L"texture\\Chehulis.png"));

	pObj->AddComponent(new CPlayerScript);
	pObj->StateMachine()->SetFSM(CAssetMgr::GetInst()->FindAsset<CFSM>(L"PlayerFSM"));

	pTempLevel->AddObject(pObj, L"Player", false);

	//Monster Object 생성
	pObj = new CGameObject;
	pObj->SetName(L"Zombie");
	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(new CCollider2D);
	pObj->AddComponent(new CStateMachine);
	pObj->AddComponent(new CAnimator2D);
	pObj->AddComponent(new CMovement);
	pObj->AddComponent(new CShadowScript);
	pObj->AddComponent(new CMonsterScript);
	pObj->AddComponent(new CZombieScript);
	pObj->AddComponent(new CZSortScript);
	pObj->AddComponent(new CEffectScript);

	pObj->Transform()->SetRelativePos(Vec3(0.f, 300.f, 0.f));
	pObj->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));

	pObj->Movement()->SetMass(1.f);
	pObj->Movement()->SetInitSpeed(50.f);
	pObj->Movement()->SetMaxSpeed(1000.f);
	pObj->Movement()->SetFrictionScale(1000.f);

	pObj->Collider2D()->SetAbsolute(true);
	pObj->Collider2D()->SetOffsetScale(Vec2(15.f, 35.f));
	pObj->Collider2D()->SetOffsetPos(Vec2(2.f, 18.f));
	//pObj->Collider2D()->SetVisible(true);

	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	CAssetMgr::GetInst()->Load<CMaterial>(L"Zombie", L"material\\Zombie.mtrl");
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Zombie"));

	pObj->StateMachine()->SetFSM(CAssetMgr::GetInst()->FindAsset<CFSM>(L"NormalMonsterFSM"));

	pTempLevel->AddObject(pObj, L"Monster", false);

	// Boss Monster 생성
	pObj = new CGameObject;
	pObj->SetName(L"Boss_Niug");

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(new CCollider2D);
	pObj->AddComponent(new CStateMachine);
	pObj->AddComponent(new CAnimator2D);
	pObj->AddComponent(new CMovement);
	pObj->AddComponent(new CShadowScript);
	pObj->AddComponent(new CMonsterScript);
	pObj->AddComponent(new CBossNiugScript);
	pObj->AddComponent(new CZSortScript);
	pObj->AddComponent(new CAfterImageScript);
	//pObj->AddComponent(new CEffectScript);

	pObj->Transform()->SetRelativePos(Vec3(-100.f, 300.f, 0.f));
	pObj->Transform()->SetRelativeScale(Vec3(300.f, 300.f, 1.f));

	pObj->Movement()->SetMass(1.f);
	pObj->Movement()->SetInitSpeed(100.f);
	pObj->Movement()->SetMaxSpeed(1000.f);
	pObj->Movement()->SetFrictionScale(1000.f);

	pObj->Collider2D()->SetAbsolute(true);
	pObj->Collider2D()->SetOffsetScale(Vec2(15.f, 35.f));
	pObj->Collider2D()->SetOffsetPos(Vec2(2.f, 18.f));
	pObj->Collider2D()->SetVisible(true);

	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	CAssetMgr::GetInst()->Load<CMaterial>(L"Boss_Niug", L"material\\Zombie.mtrl");
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Boss_Niug"));

	pObj->StateMachine()->SetFSM(CAssetMgr::GetInst()->FindAsset<CFSM>(L"Boss_NiugFSM"));

	pTempLevel->AddObject(pObj, L"Monster", false);

	//Altar Object 생성
	pObj = new CGameObject;
	pObj->SetName(L"Jail_Stone");

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(new CCollider2D);
	pObj->AddComponent(new CMovement);
	pObj->AddComponent(new CShadowScript);
	pObj->AddComponent(new CNormalObjScript);
	pObj->AddComponent(new CZSortScript);

	pObj->Transform()->SetRelativePos(Vec3(-100.f, 50.f, 0.f));
	pObj->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 1.f));

	pObj->Collider2D()->SetAbsolute(true);
	pObj->Collider2D()->SetOffsetScale(Vec2(92.f, 46.f));
	pObj->Collider2D()->SetOffsetPos(Vec2(0.f, -6.f));
	//pObj->Collider2D()->SetVisible(true);

	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));

	pTempLevel->AddObject(pObj, L"Monster", false);

	// Camera LayerCheck 와 충돌설정에서 LayerCheck 시 m_CurLevel == nullptr 이므로 Idx번호로 
	// 충돌 설정
	CCollisionMgr::GetInst()->LayerCheck(2,3);
	CCollisionMgr::GetInst()->LayerCheck(3,3);
	CCollisionMgr::GetInst()->LayerCheck(2,11);
	CCollisionMgr::GetInst()->LayerCheck(2,12);
	CCollisionMgr::GetInst()->LayerCheck(3,12);
	CCollisionMgr::GetInst()->LayerCheck(3,22);
	CCollisionMgr::GetInst()->LayerCheck(3,20);
	CCollisionMgr::GetInst()->LayerCheck(21,25);
	CCollisionMgr::GetInst()->LayerCheck(2,26);

	CLevelMgr::GetInst()->ChangeLevel(pTempLevel, LEVEL_STATE::STOP);

	CLevelSaveLoad::SaveLevel(pTempLevel, L"level\\temp.lv");
}
