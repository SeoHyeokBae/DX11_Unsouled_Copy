#include "pch.h"
#include "CDevice.h"

#include "CConstBuffer.h"

CDevice::CDevice()
	: m_hRenderWnd(nullptr)
	, m_arrCB{}
	, m_arrRS{}
	, m_arrDS{}
	, m_arrBS{}
	, m_arrSampler{}
{
}

CDevice::~CDevice()
{
	Delete_Array(m_arrCB);
}

int CDevice::init(HWND _hWnd, Vec2 _vResolution)
{
	// ��� ������
	m_hRenderWnd = _hWnd;

	// ���� �ػ� 
	m_vRenderResolution = _vResolution;

	// ��ġ �ʱ�ȭ
	D3D_FEATURE_LEVEL eLevel = D3D_FEATURE_LEVEL_11_0;

	if (FAILED(D3D11CreateDevice(nullptr
								, D3D_DRIVER_TYPE_HARDWARE, nullptr
								, D3D11_CREATE_DEVICE_DEBUG
								, nullptr, 0, D3D11_SDK_VERSION
								, m_Device.GetAddressOf(), &eLevel, m_Context.GetAddressOf())))	// GetAddressOf() ����Ʈ������ ���������� ����
	{
		MessageBox(nullptr, L"Device, Context ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
		return E_FAIL;
	}

	// ����ü�� ���� 
	if (FAILED(CreateSwapChain()))
	{
		MessageBox(nullptr, L"SwapChain ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
		return E_FAIL;
	}

	// ����Ÿ��, ����Ÿ�� ��, �������ٽ� Ÿ��, ���� ���ٽ� �� ����
	if (FAILED(CreateTargetView()))
	{
		MessageBox(nullptr, L"Ÿ�� �� View ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
		return E_FAIL;
	}

	if (FAILED(CreateRasterizerState()))
	{
		MessageBox(nullptr, L"Rasterizer State ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
		return E_FAIL;
	}

	if (FAILED(CreateDepthStencilState()))
	{
		MessageBox(nullptr, L"DepthStencil State ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
		return E_FAIL;
	}


	if (FAILED(CreateBlendState()))
	{
		MessageBox(nullptr, L"Blend State ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
		return E_FAIL;
	}

	if (FAILED(CreateSamplerState()))
	{
		MessageBox(nullptr, L"Sampler State ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
		return E_FAIL;
	}

	// ViewPort ����
	D3D11_VIEWPORT ViewportDesc = {};

	ViewportDesc.MinDepth = 0;
	ViewportDesc.MaxDepth = 1.f;

	ViewportDesc.TopLeftX = 0;
	ViewportDesc.TopLeftY = 0;
	ViewportDesc.Width = m_vRenderResolution.x;
	ViewportDesc.Height = m_vRenderResolution.y;

	CONTEXT->RSSetViewports(1, &ViewportDesc);

	if (FAILED(CreateConstBuffer()))
	{
		MessageBox(nullptr, L"������� ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

void CDevice::ClearRenderTarget(float(&Color)[4])
{
	m_Context->ClearRenderTargetView(m_RTView.Get(), Color );
	m_Context->ClearDepthStencilView(m_DSView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);	// ���� 1�� �ʱ�ȭ -> �Ǿ� ���θ��� �ؽ��İ� �ȱ׷����°��� ����
}

void CDevice::Present()
{
	m_SwapChain->Present(0, 0);
}

int CDevice::CreateSwapChain()
{
	// SwapChain ���� ����ü
	DXGI_SWAP_CHAIN_DESC tDesc = {};

	// SwapChain �� �����ϴ� Buffer(RenderTarget) �� ���� ����
	tDesc.BufferCount = 1;
	tDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	tDesc.BufferDesc.Width = (UINT)m_vRenderResolution.x;
	tDesc.BufferDesc.Height = (UINT)m_vRenderResolution.y;
	tDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	tDesc.BufferDesc.RefreshRate.Denominator = 1;
	tDesc.BufferDesc.RefreshRate.Numerator = 60;
	tDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	tDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	tDesc.Flags = 0;

	tDesc.SampleDesc.Count = 1;
	tDesc.SampleDesc.Quality = 0;

	tDesc.Windowed = true; // â���
	tDesc.OutputWindow = m_hRenderWnd; // SwapChain �� ��� ������ ����


	// ����ü�� ��������� ������ �ִ� Factory �� �����Ѵ�.
	ComPtr<IDXGIDevice>		pIdxgiDevice = nullptr;
	ComPtr<IDXGIAdapter>	pAdapter = nullptr;
	ComPtr<IDXGIFactory>	pFactory = nullptr;

	m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)pIdxgiDevice.GetAddressOf());
	pIdxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pAdapter.GetAddressOf());
	pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf());

	// SwapChain ����
	if (FAILED(pFactory->CreateSwapChain(m_Device.Get(), &tDesc, m_SwapChain.GetAddressOf())))
	{
		return E_FAIL;
	}


	return S_OK;
}

int CDevice::CreateTargetView()
{
	/*ID3D11Resource;
	ID3D11Buffer;
	ID3D11Texture2D;*/

	// ����Ÿ�� �ؽ��ĸ� ����ü�����κ��� ���´�.
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)m_RTTex.GetAddressOf());

	// RenderTargetView
	m_Device->CreateRenderTargetView(m_RTTex.Get(), nullptr, m_RTView.GetAddressOf());

	// View?
	// RenderTargetView
	// DepthStencilView
	// ShaderResourceView
	// UnorderedAccessView

	// ���� ü�� ������ ����Ÿ�Ͽ� �ؽ�ó�� ���������� 
	// �������ٽ� �ؽ�ó�� �ƹ��� �������� �ʱ� ������ ���� ���� �ؾ���
	// �������ٽ� �ؽ�ó�� ����� �� 
	// DTV + RTV �� OM�� ����
	 
	// DepthStencilTexture ����
	D3D11_TEXTURE2D_DESC Desc = {};

	// �ȼ� ������ Depth 3����Ʈ Stencil 1����Ʈ
	Desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// DepthStencilState �ؽ��� �ػ󵵴� �ݵ�� RenderTargetTexture �� �����ؾ��Ѵ�.
	Desc.Width = (UINT)m_vRenderResolution.x;
	Desc.Height = (UINT)m_vRenderResolution.y;

	// DepthStencil �뵵�� �ؽ���
	Desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	// CPU ���� �Ұ�
	Desc.CPUAccessFlags = 0;
	Desc.Usage = D3D11_USAGE_DEFAULT;

	// ���ø�
	Desc.SampleDesc.Count = 1;
	Desc.SampleDesc.Quality = 0;

	// ������Ƽ ������ �纻 ��������
	Desc.MipLevels = 1;
	Desc.MiscFlags = 0;

	Desc.ArraySize = 1;

	if (FAILED(m_Device->CreateTexture2D(&Desc, nullptr, m_DSTex.GetAddressOf())))
	{
		return E_FAIL;
	}

	// DepthStencilView
	m_Device->CreateDepthStencilView(m_DSTex.Get(), nullptr, m_DSView.GetAddressOf());

	// OM(Output Merge State) �� RenderTargetTexture �� DepthStencilTexture �� �����Ѵ�.
	m_Context->OMSetRenderTargets(1, m_RTView.GetAddressOf(), m_DSView.Get());

	return S_OK;
}

int CDevice::CreateRasterizerState()
{
	m_arrRS[(UINT)RS_TYPE::CULL_BACK] = nullptr;	// �⺻ ���� nullptr �ʱ�ȭ

	HRESULT result = S_OK;

	D3D11_RASTERIZER_DESC tDesc = {};

	// CULL_FRONT Ÿ�� = �ո� ����x 
	tDesc.CullMode = D3D11_CULL_FRONT;
	tDesc.FillMode = D3D11_FILL_SOLID;
	result = DEVICE->CreateRasterizerState(&tDesc, m_arrRS[(UINT)RS_TYPE::CULL_FRONT].GetAddressOf());
	if (FAILED(result))
		return E_FAIL;

	// CULL_NONE Ÿ�� = �ո� �޸� ��� ����
	tDesc.CullMode = D3D11_CULL_NONE;
	tDesc.FillMode = D3D11_FILL_SOLID;
	result = DEVICE->CreateRasterizerState(&tDesc, m_arrRS[(UINT)RS_TYPE::CULL_NONE].GetAddressOf());
	if (FAILED(result))
		return E_FAIL;

	// CULL_WIREFRAME Ÿ�� = ��� ä�� ������ �ƴ� ������(����)�� �ո� �޸� ��� ����
	tDesc.CullMode = D3D11_CULL_NONE;
	tDesc.FillMode = D3D11_FILL_WIREFRAME;
	result = DEVICE->CreateRasterizerState(&tDesc, m_arrRS[(UINT)RS_TYPE::WIRE_FRAME].GetAddressOf());
	if (FAILED(result))
		return E_FAIL;

	return S_OK;
}

int CDevice::CreateDepthStencilState()
{
	HRESULT hr = S_OK;

	m_arrDS[(UINT)DS_TYPE::LESS] = nullptr;

	D3D11_DEPTH_STENCIL_DESC tDesc = {};

	// Less = ������ z���� ���� ������ z�� �� ���� ��, �� ���� ������ �� �տ� �������� z��(����)�� �����Ѵ�, �����ϸ� �������� ����

	// Less Equal = ���� ���̰� ������ ���̺��� �� ���ų�(�տ� �ְų�) ���� ��쿡 z���� �����Ѵ�
	// ex) ���� ���̶�� ���߿� �׸��� �ﰢ���� �� �տ� �´�
	tDesc.DepthEnable = true;  // ���� ���۸� Ȱ��ȭ ���� ���� false�� �����Ѵٸ� ���� ���� ���� �׸��� ������� ȭ�鿡 ��Ÿ��
	tDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	tDesc.StencilEnable = false;
	hr = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::LESS_EQUAL].GetAddressOf());
	if (FAILED(hr)) return E_FAIL;

	// Greater
	tDesc.DepthEnable = true;	
	tDesc.DepthFunc = D3D11_COMPARISON_GREATER;
	tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	tDesc.StencilEnable = false;
	hr = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::GRATER].GetAddressOf());
	if (FAILED(hr)) return E_FAIL;

	// Greater Equal
	tDesc.DepthEnable = true;
	tDesc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
	tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	tDesc.StencilEnable = false;
	hr = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::GRATER_EQUAL].GetAddressOf());
	if (FAILED(hr)) return E_FAIL;

	// No Test
	tDesc.DepthEnable = false;
	tDesc.DepthFunc = D3D11_COMPARISON_NEVER;
	tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	tDesc.StencilEnable = false;
	hr = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::NO_TEST].GetAddressOf());

	// No Write
	tDesc.DepthEnable = true;
	tDesc.DepthFunc = D3D11_COMPARISON_LESS;
	tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	tDesc.StencilEnable = false;
	hr = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::NO_WRITE].GetAddressOf());
	if (FAILED(hr)) return E_FAIL;

	// NoTest NoWrite
	tDesc.DepthEnable = false;
	tDesc.DepthFunc = D3D11_COMPARISON_NEVER;
	tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	tDesc.StencilEnable = false;
	hr = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::NO_TEST_NO_WRITE].GetAddressOf());
	if (FAILED(hr)) return E_FAIL;

	return S_OK;
}

int CDevice::CreateBlendState()
{
	m_arrBS[(UINT)BS_TYPE::DEFAULT] = nullptr;



	D3D11_BLEND_DESC tDesc = {};

	// AlphaBlend	
	// src(RGB) * a + dest(RGB) * (1 - a) = 
	tDesc.AlphaToCoverageEnable = false;	// ����, ��Ǯ�� ��� ���� ���� �ڿ� �������� �κ�
	tDesc.IndependentBlendEnable = false;	// Ÿ���� �������϶� ������ ��� ����

	tDesc.RenderTarget[0].BlendEnable = true;
	tDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;				// ȥ�� ������
	tDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;			// �ؽ��İ� ���� �ִ� ���İ� a
	tDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;	// 1 - �ؽ��� ���İ� (1 - a)

	tDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;		// ���� ���� ȥ�� ������
	tDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;			// ���� ���� ȥ���� �ҽ� ȥ�� ���
	tDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;			// ���� ���� ȥ���� dest ȥ�� ���

	tDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	DEVICE->CreateBlendState(&tDesc, m_arrBS[(UINT)BS_TYPE::ALPHA_BLEND].GetAddressOf());

	// One_One
	// AlphaBlend ����
	// ��� ������( a )�� 0�� �ƴϰ� ������ RGB(0 0 0)�ϰ�� 
	// src(���� 0,0,0) * 1 + dest(������ 0.25,0.25,0.25 ) * 1 = 0.25, 0.25, 0.25 src�� ������ ����� �������� ��ġ����
	// but ��ü ���� + 0.25 ���� ���� �������� 0.f ��� ��ü �� ���� ���� ������ ?
	tDesc.AlphaToCoverageEnable = false;
	tDesc.IndependentBlendEnable = false;

	tDesc.RenderTarget[0].BlendEnable = true;
	tDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	tDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;		// �ؽ��� ���� ��� = 1
	tDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;		// dest ���� ��� = 1

	tDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	tDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	tDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

	tDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	DEVICE->CreateBlendState(&tDesc, m_arrBS[(UINT)BS_TYPE::ONE_ONE].GetAddressOf());

	return S_OK;
}

int CDevice::CreateSamplerState()
{
	D3D11_SAMPLER_DESC tDesc = {};

	tDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.Filter = D3D11_FILTER_ANISOTROPIC;

	tDesc.MinLOD = 0;
	tDesc.MaxLOD = 1;

	DEVICE->CreateSamplerState(&tDesc, m_arrSampler[0].GetAddressOf());


	tDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;

	tDesc.MinLOD = 0;
	tDesc.MaxLOD = 1;

	DEVICE->CreateSamplerState(&tDesc, m_arrSampler[1].GetAddressOf());

	CONTEXT->VSSetSamplers(0, 1, m_arrSampler[0].GetAddressOf());
	CONTEXT->HSSetSamplers(0, 1, m_arrSampler[0].GetAddressOf());
	CONTEXT->DSSetSamplers(0, 1, m_arrSampler[0].GetAddressOf());
	CONTEXT->GSSetSamplers(0, 1, m_arrSampler[0].GetAddressOf());
	CONTEXT->PSSetSamplers(0, 1, m_arrSampler[0].GetAddressOf());

	CONTEXT->VSSetSamplers(1, 1, m_arrSampler[1].GetAddressOf());
	CONTEXT->HSSetSamplers(1, 1, m_arrSampler[1].GetAddressOf());
	CONTEXT->DSSetSamplers(1, 1, m_arrSampler[1].GetAddressOf());
	CONTEXT->GSSetSamplers(1, 1, m_arrSampler[1].GetAddressOf());
	CONTEXT->PSSetSamplers(1, 1, m_arrSampler[1].GetAddressOf());

	return S_OK;
}

int CDevice::CreateConstBuffer()
{
	m_arrCB[(UINT)CB_TYPE::TRANSFORM] = new CConstBuffer;
	m_arrCB[(UINT)CB_TYPE::TRANSFORM]->Create(sizeof(tTransform), 1);


	return S_OK;
}
