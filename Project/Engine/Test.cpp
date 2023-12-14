#include "pch.h"
#include "Test.h"

#include "CDevice.h"

#include "CPathMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"

#include "CGameObject.h"

#include "CMesh.h"
#include "CGraphicsShader.h"

#include "CTransform.h"
#include "CMeshRender.h"


CGameObject* g_Object = nullptr;

tTransform g_Transform = { Vec4(0.f, 0.f, 0.f, 0.f), Vec4(1.f, 1.f, 1.f, 1.f) };

CMesh* g_RectMesh = nullptr;
CMesh* g_CircleMesh = nullptr;

CGraphicsShader* g_Shader = nullptr;


int TestInit()
{
	// ���������� �ﰢ�� ��ġ ����
	//   0(Red)-- 1(Blue)	     
	//    |   \   |	     
	//   3(G)---- 2(Magenta)  
	Vtx arrVtx[4] = {};
	arrVtx[0].vPos = Vec3(-0.5f, 0.5f, 0.f);
	arrVtx[0].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrVtx[0].vUV = Vec2(0.f, 0.f);

	arrVtx[1].vPos = Vec3(0.5f, 0.5f, 0.f);
	arrVtx[1].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrVtx[1].vUV = Vec2(0.f, 0.f);

	arrVtx[2].vPos = Vec3(0.5f, -0.5f, 0.f);
	arrVtx[2].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrVtx[2].vUV = Vec2(0.f, 0.f);

	arrVtx[3].vPos = Vec3(-0.5f, -0.5f, 0.f);
	arrVtx[3].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrVtx[3].vUV = Vec2(0.f, 0.f);

	// �ε��� ����
	UINT arrIdx[6] = {};
	arrIdx[0] = 0;
	arrIdx[1] = 1;
	arrIdx[2] = 2;

	arrIdx[3] = 0;
	arrIdx[4] = 2;
	arrIdx[5] = 3;

	g_RectMesh = new CMesh();
	g_RectMesh->Create(arrVtx, 4, arrIdx, 6);

	// =================
	// CircleMesh �����
	// =================
	vector<Vtx> vecVtx;
	vector<UINT> vecIdx;
	Vtx v;

	// �߽���
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vUV = Vec2(0.5f, 0.5f);

	vecVtx.push_back(v);

	UINT iSlice = 40;
	float fTheta = 0.f;
	float fRadius = 0.5f;

	for (int i = 0; i <= iSlice; i++)
	{
		fTheta = (XM_2PI / iSlice) * i;
		
		v.vPos = Vec3(fRadius * cosf(fTheta), fRadius * sinf(fTheta), 0.f);
		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
		v.vUV = Vec2(0.f, 0.f);

		vecVtx.push_back(v);
	}
	
	for (int i = 0; i < iSlice; i++)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i+2);
		vecIdx.push_back(i+1);
	}
	g_CircleMesh = new CMesh;
	g_CircleMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());






	// Shader ����
	g_Shader = new CGraphicsShader;
	g_Shader->CreateVerTexShader(L"shader\\std2d.fx", "VS_Std2D");
	g_Shader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");


	// GameObject 1�� ����
	g_Object = new CGameObject;

	g_Object->AddComponent(new CTransform);
	g_Object->AddComponent(new CMeshRender);

	//g_Object->Transform()->SetRelativePos(Vec3(-0.5f, 0.f, 0.f));
	g_Object->Transform()->SetRelativeScale(Vec3(1.5f, 1.5f, 1.5f));

	g_Object->MeshRender()->SetMesh(g_RectMesh);
	g_Object->MeshRender()->SetShader(g_Shader);
	
	return S_OK;
}

void Tick()
{
	g_Object->tick();
	g_Object->finaltick();
}

void Render()
{
	// ������ ȭ�� Ŭ����
	float ClearColor[4] = { 0.3f, 0.3f, 0.3f, 1.f };
	CDevice::GetInst()->ClearRenderTarget(ClearColor);

	g_Object->render();

	CDevice::GetInst()->Present();
}

void TestProgress()
{
	Tick();

	Render(); 
}

void TestRelease()
{
	if (nullptr != g_RectMesh)
	{
		delete g_RectMesh;
	}

	if (nullptr != g_CircleMesh)
	{
		delete g_CircleMesh;
	}

	delete g_Shader;

	delete g_Object;
}
