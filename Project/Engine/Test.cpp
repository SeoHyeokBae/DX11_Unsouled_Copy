#include "pch.h"
#include "Test.h"

#include "CDevice.h"

#include "CPathMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"

// �ﰢ�� �׸���
//Vtx g_vtx[3] = {};

// �簢�� �׸���
Vtx		g_vtx[4] = {};
UINT	g_Idx[6] = {};

// ������ �����ϴ� ��������
ComPtr<ID3D11Buffer>	g_VB = nullptr;

// �ε����� �����ϴ� ����
ComPtr<ID3D11Buffer>	g_IB = nullptr;



// InputLayout �����ϳ��� ������ �˸��� ��ü
ComPtr<ID3D11InputLayout> g_Layout = nullptr;

// ���ؽ�/�ȼ� ���̴� GPU�� �� �� �ְ� HLSL �Լ� ������ �� Blob�� ���̳ʸ� �ڵ�� ����
ComPtr<ID3DBlob> g_VSBlob = nullptr;
ComPtr<ID3DBlob> g_PSBlob = nullptr;
ComPtr<ID3DBlob> g_ErrBlob = nullptr;

ComPtr<ID3D11VertexShader> g_VS = nullptr;
ComPtr<ID3D11PixelShader> g_PS = nullptr;

int TestInit()
{
	// ���������� �ﰢ�� ��ġ ����
	//   0(Red)-- 1(Blue)	     
	//    |   \   |	     
	//   3(G)---- 2(Magenta)  
	g_vtx[0].vPos = Vec3(-0.5f, 0.5f, 0.f);
	g_vtx[0].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	g_vtx[0].vUV = Vec2(0.f, 0.f);

	g_vtx[1].vPos = Vec3(0.5f, 0.5f, 0.f);
	g_vtx[1].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	g_vtx[1].vUV = Vec2(0.f, 0.f);

	g_vtx[2].vPos = Vec3(0.5f, -0.5f, 0.f);
	g_vtx[2].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	g_vtx[2].vUV = Vec2(0.f, 0.f);

	g_vtx[3].vPos = Vec3(-0.5f, -0.5f, 0.f);
	g_vtx[3].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	g_vtx[3].vUV = Vec2(0.f, 0.f);


	// ���ؽ� ���� ����
	D3D11_BUFFER_DESC BufferDesc = {};

	BufferDesc.ByteWidth = sizeof(Vtx) * 4;
	BufferDesc.StructureByteStride = sizeof(Vtx);
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// ���ۿ� ������ ���� ����
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	BufferDesc.Usage = D3D11_USAGE_DYNAMIC;

	// g_Vtx �迭�� �����͸� �ʱ� �����ͷ� ����
	D3D11_SUBRESOURCE_DATA tSubData = {};
	tSubData.pSysMem = g_vtx;	// ������ ���ÿ� �ʱ�ȭ

	// ���ؽ� ���� ����
	if (FAILED(DEVICE->CreateBuffer(&BufferDesc, &tSubData, g_VB.GetAddressOf())))
	{
		MessageBox(nullptr, L"���ؽ� ���� ���� ����", L"TestInit ����", MB_OK);
		return E_FAIL;
	}

	// ���ؽ� ���̴�
	// HLSL ���ؽ� ���̴� �Լ� ������
	wstring strFilePath = CPathMgr::GetContentPath();

	if (FAILED(D3DCompileFromFile(wstring(strFilePath + L"shader\\std2d.fx").c_str()
		, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, "VS_Std2D", "vs_5_0", D3DCOMPILE_DEBUG, 0
		, g_VSBlob.GetAddressOf(), g_ErrBlob.GetAddressOf()))) // ���̳ʸ��ڵ带 g_VSBlob�� ���� // ������ ���� ���ڿ� ErrBlob�� ����
	{
		if (nullptr != g_ErrBlob)
		{
			char* pErrMsg = (char*)g_ErrBlob->GetBufferPointer();
			MessageBoxA(nullptr, pErrMsg, "Shader Compile Failed!!", MB_OK);
		}

		return E_FAIL;
	}

	// ���ؽ� ���̴� ����
	DEVICE->CreateVertexShader(g_VSBlob->GetBufferPointer()
		, g_VSBlob->GetBufferSize(), nullptr
		, g_VS.GetAddressOf());


	// �ε��� ����
	g_Idx[0] = 0;
	g_Idx[1] = 1;
	g_Idx[2] = 2;

	g_Idx[3] = 0;
	g_Idx[4] = 2;
	g_Idx[5] = 3;

	// �ε��� ���� ����
	BufferDesc = {};

	BufferDesc.ByteWidth = sizeof(UINT) * 6;
	BufferDesc.StructureByteStride = sizeof(UINT);
	BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	// ���ۿ� ������ ���� �Ұ���
	BufferDesc.CPUAccessFlags = 0;
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;

	// g_Idx �迭�� �����͸� �ʱ� �����ͷ� ����
	tSubData = {};
	tSubData.pSysMem = g_Idx;

	// �ε��� ���� ����
	if (FAILED(DEVICE->CreateBuffer(&BufferDesc, &tSubData, g_IB.GetAddressOf())))
	{
		MessageBox(nullptr, L"�ε��� ���� ���� ����", L"TestInit ����", MB_OK);
		return E_FAIL;
	}

	//���̾ƿ�
	// ���� ��������(Layout) ����
	D3D11_INPUT_ELEMENT_DESC arrElement[3] = {};

	arrElement[0].InputSlot = 0;
	arrElement[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	arrElement[0].SemanticName = "POSITION";
	arrElement[0].SemanticIndex = 0;
	arrElement[0].InstanceDataStepRate = 0;
	arrElement[0].AlignedByteOffset = 0;
	arrElement[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;

	arrElement[1].InputSlot = 0;
	arrElement[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	arrElement[1].SemanticName = "COLOR";
	arrElement[1].SemanticIndex = 0;
	arrElement[1].InstanceDataStepRate = 0;
	arrElement[1].AlignedByteOffset = 12;
	arrElement[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

	arrElement[2].InputSlot = 0;
	arrElement[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	arrElement[2].SemanticName = "TEXCOORD";
	arrElement[2].SemanticIndex = 0;
	arrElement[2].InstanceDataStepRate = 0;
	arrElement[2].AlignedByteOffset = 28;
	arrElement[2].Format = DXGI_FORMAT_R32G32_FLOAT;

	// �ȼ� ���̴� ����
	// �ȼ� ���̴� ������
	if (FAILED(D3DCompileFromFile(wstring(strFilePath + L"shader\\std2d.fx").c_str()
		, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, "PS_Std2D", "ps_5_0", D3DCOMPILE_DEBUG, 0
		, g_PSBlob.GetAddressOf(), g_ErrBlob.GetAddressOf())))
	{
		if (nullptr != g_ErrBlob)
		{
			char* pErrMsg = (char*)g_ErrBlob->GetBufferPointer();
			MessageBoxA(nullptr, pErrMsg, "Shader Compile Failed!!", MB_OK);
		}

		return E_FAIL;
	}

	DEVICE->CreatePixelShader(g_PSBlob->GetBufferPointer()
		, g_PSBlob->GetBufferSize(), nullptr
		, g_PS.GetAddressOf());


	// Layout ����
	DEVICE->CreateInputLayout(arrElement, 3
		, g_VSBlob->GetBufferPointer()
		, g_VSBlob->GetBufferSize()
		, g_Layout.GetAddressOf());

	return S_OK;
}

void Tick()
{
	if (KEY_PRESSED(KEY::LEFT))
	{
		for (int i = 0; i < 4; ++i)
		{
			g_vtx[i].vPos.x -= DT;
		}
	}

	if (KEY_PRESSED(KEY::RIGHT))
	{
		for (int i = 0; i < 4; ++i)
		{
			g_vtx[i].vPos.x += DT;
		}
	}

	if (KEY_PRESSED(KEY::UP))
	{
		for (int i = 0; i < 4; ++i)
		{
			g_vtx[i].vPos.y += DT;
		}
	}

	if (KEY_PRESSED(KEY::DOWN))
	{
		for (int i = 0; i < 4; ++i)
		{
			g_vtx[i].vPos.y -= DT;
		}
	}

	//// SystemMem -> GPUMem
	D3D11_MAPPED_SUBRESOURCE tSub = {};

	CONTEXT->Map(g_VB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
	memcpy(tSub.pData, g_vtx, sizeof(Vtx) * 4);
	CONTEXT->Unmap(g_VB.Get(), 0);
}

void Render()
{
	float ClearColor[4] = { 0.3f, 0.3f, 0.3f, 1.f };
	CDevice::GetInst()->ClearRenderTarget(ClearColor);

	// �ﰢ�� �׸���
	UINT iStride = sizeof(Vtx);
	UINT iOffset = 0;
	CONTEXT->IASetVertexBuffers(0, 1, g_VB.GetAddressOf(), &iStride, &iOffset);
	CONTEXT->IASetIndexBuffer(g_IB.Get(), DXGI_FORMAT_R32_UINT, 0);
	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	CONTEXT->IASetInputLayout(g_Layout.Get());


	CONTEXT->VSSetShader(g_VS.Get(), 0, 0);
	CONTEXT->PSSetShader(g_PS.Get(), 0, 0);

	// Draw �� ������ ���� �����̱� ������ ������ �ٲ� �������
	CONTEXT->DrawIndexed(6, 0, 0);

	CDevice::GetInst()->Present();
}

void TestProgress()
{
	Tick();

	Render();
}

void TestRelease()
{
}
