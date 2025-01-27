#include "pch.h"
#include "CMesh.h"

#include "CDevice.h"

CMesh::CMesh(bool _Engine)
	: CAsset(ASSET_TYPE::MESH, _Engine)
	, m_VtxCount(0)
	, m_VtxSysMem(nullptr)
	, m_IdxSysMem(nullptr)
{
}

CMesh::~CMesh()
{
	if (nullptr != m_VtxSysMem)
		delete m_VtxSysMem;

	if (nullptr != m_IdxSysMem)
		delete m_IdxSysMem;
}

int CMesh::Create(void* _Vtx, UINT _VtxCount, void* _Idx, UINT _IdxCount)
{
	m_VtxCount = _VtxCount;
	m_IdxCount = _IdxCount;

	// 버텍스 버퍼 생성
	m_VBDesc = {};

	m_VBDesc.ByteWidth = sizeof(Vtx) * _VtxCount;
	m_VBDesc.StructureByteStride = sizeof(Vtx);
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// 버퍼에 데이터 쓰기 가능
	m_VBDesc.CPUAccessFlags = 0;			// 상수버퍼 사용으로 정점 고정 D3D11_CPU_ACCESS_WRITE;
	m_VBDesc.Usage = D3D11_USAGE_DEFAULT;	// 상수버퍼 사용으로 정점 고정 D3D11_USAGE_DYNAMIC;

	// g_Vtx 배열의 데이터를 초기 데이터로 설정
	D3D11_SUBRESOURCE_DATA tSubData = {};
	tSubData.pSysMem = _Vtx;	// 생성과 동시에 초기화

	// 버텍스 버퍼 생성
	if (FAILED(DEVICE->CreateBuffer(&m_VBDesc, &tSubData, m_VB.GetAddressOf())))
	{
		MessageBox(nullptr, L"버텍스 버퍼 생성 실패", L"오류", MB_OK);
		return E_FAIL;
	}

	// 인덱스 버퍼 생성
	m_IBDesc = {};

	m_IBDesc.ByteWidth = sizeof(UINT) * _IdxCount;
	m_IBDesc.StructureByteStride = sizeof(UINT);
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	// 버퍼에 데이터 쓰기 불가능
	m_IBDesc.CPUAccessFlags = 0;
	m_IBDesc.Usage = D3D11_USAGE_DEFAULT;

	// g_Idx 배열의 데이터를 초기 데이터로 설정
	tSubData = {};
	tSubData.pSysMem = _Idx;

	// 인덱스 버퍼 생성
	if (FAILED(DEVICE->CreateBuffer(&m_IBDesc, &tSubData, m_IB.GetAddressOf())))
	{
		MessageBox(nullptr, L"인덱스 버퍼 생성 실패", L"오류", MB_OK);
		return E_FAIL;
	}


	// 원본 정점 정보 및 인덱스 정보를 동적할당한 곳에다가 저장시켜두고 관리
	m_VtxSysMem = new Vtx[m_VtxCount];
	m_IdxSysMem = new UINT[m_IdxCount];

	memcpy(m_VtxSysMem, _Vtx, sizeof(Vtx) * m_VtxCount);
	memcpy(m_IdxSysMem, _Vtx, sizeof(UINT) * m_IdxCount);

	return 0;
}

void CMesh::UpdateData()
{
	UINT iStride = sizeof(Vtx);
	UINT iOffset = 0;

	CONTEXT->IASetVertexBuffers(0, 1, m_VB.GetAddressOf(), &iStride, &iOffset);
	CONTEXT->IASetIndexBuffer(m_IB.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void CMesh::render()
{
	UpdateData();	// 버텍스버퍼 , 인덱스버퍼 세팅

	CONTEXT->DrawIndexed(m_IdxCount, 0, 0);

}


// 인스턴싱
// 렌더링 파이프라인 단계를 여러번 거쳐서 객체를 찍는것 보다 한번에 찍는게 더 빠르다
void CMesh::render_asparticle(UINT _ParticleCount)
{
	UpdateData();

	// 렌더링 파이프라인 안에서 자체적으로 _ParticleCount 만큼 반복문을 수행한다
	// 함수가 한번만 수행되기 때문에 몇 번째 객체가 그려지는지 구별할 수 있는 요소가 추가적으로 필요
	//  - 원래는 ScalarParmeter를 통해 몇번째 파라미터인지 구별할 수 있었다
	// Vertex Shader의 입력으로 새로운 값을 받아올 수 있다
	// -uint iInsID : SV_InstanceID
	CONTEXT->DrawIndexedInstanced(m_IdxCount, _ParticleCount, 0, 0, 0);
	
}

