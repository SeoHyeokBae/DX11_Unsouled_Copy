#include "pch.h"
#include "CAfterImageScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
//#include <Engine/CGameObject.h>

CAfterImageScript::CAfterImageScript()
	: CScript(AFTERIMAGESCRIPT)
	, m_AfterImgObj(nullptr)
	, m_CurIdx(0)
	, m_FadeSpeed(1.f)

{
}

CAfterImageScript::~CAfterImageScript()
{
}


void CAfterImageScript::begin()
{
	m_AfterImgObj = new CGameObject;
	m_AfterImgObj->SetName(L"AfterImage");
	m_AfterImgObj->AddComponent(new CTransform);
	Vec3 vPos = Transform()->GetRelativePos();
	//m_AfterImgObj->Transform()->SetRelativePos(-vPos);
	GetOwner()->AddChild(m_AfterImgObj);
	GamePlayStatic::SpawnGameObject(m_AfterImgObj, 11); // 추후 변경
	for (int i = 0; i < POOLCOUNT; i++)
	{
		wchar_t number[128] = {};
		wchar_t mtrl[256] = {};

		swprintf_s(number, L"AfterImageChild_%d", i);
		swprintf_s(mtrl, L"material\\AfterImage\\AfterImage%d.mtrl", i);
		CGameObject* sprite = new CGameObject;
		sprite->SetName(number);
		
		sprite->AddComponent(new CTransform);
		sprite->AddComponent(new CMeshRender);
		sprite->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
		CAssetMgr::GetInst()->Load<CMaterial>(number, mtrl);
		sprite->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(number));

		sprite->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));

		m_vSprite.push_back(make_pair(false, sprite));
		m_AfterImgObj->AddChild(m_vSprite[i].second);
		GamePlayStatic::SpawnGameObject(m_vSprite[i].second, 11); // 추후 변경
	}
	


}

void CAfterImageScript::tick()
{
	Vec3 vOwnerPos = Transform()->GetRelativePos();
	m_AfterImgObj->Transform()->SetRelativePos(-vOwnerPos);
	
	// 이미지 초기화 
	if (GetOwner()->IsAfterImgAct())
	{
		Init();
	}

	Update();
}

void CAfterImageScript::Update()
{
	// 색상 업데이트 , 알파 감소
	if (0 == m_vFrm.size())
		return;

	for (size_t i = 0; i < m_vFrm.size(); i++)
	{
		m_vFrm[i].fAlhpa -= m_FadeSpeed * DT;
		Vec4 vColor = Vec4(0.0f, 0.0f, 0.0f, m_vFrm[i].fAlhpa);
		m_vSprite[i].second->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, vColor);
	}

	if (m_vFrm[0].fAlhpa < 0.f)
	{
		Clear();
	}

}


void CAfterImageScript::Init()
{
	for (size_t i = 0; i < m_vFrm.size(); i++)
	{
		Vec3 vPos = m_vFrm[i].vPos;
		Vec2 vLT = m_vFrm[i].AnimFrm.vLeftTop;
		Vec2 vBg = m_vFrm[i].AnimFrm.vBackground;
		Vec2 vSize = m_vFrm[i].AnimFrm.vSlice;

		Vec4 vColor = Vec4(0.0f, 0.0f, 0.0f, m_vFrm[i].fAlhpa);

		m_vSprite[i].second->Transform()->SetRelativePos(vPos);
		m_vSprite[i].second->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_1, m_pTexture);
		m_vSprite[i].second->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_0, vLT); //UV
		m_vSprite[i].second->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_1, vBg); //Bg
		m_vSprite[i].second->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_2, vSize); //Size
		m_vSprite[i].first = true; // 정보입력완료
	}

}



void CAfterImageScript::Clear()
{
	for (size_t i = 0; i < POOLCOUNT; i++)
	{
		Vec2 zero = Vec2(0.f, 0.f);
		Vec4 col = Vec4(0.f, 0.f, 0.f, 0.8);
		m_vSprite[i].first = false;
		m_vSprite[i].second->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_1, nullptr);
		m_vSprite[i].second->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_0, zero); //UV
		m_vSprite[i].second->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_1, zero); //Bg
		m_vSprite[i].second->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_2, zero); //Size
		m_vSprite[i].second->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, col); //색상
	}

	m_vFrm.clear();
	vector<tAftInfo> temp;
	m_vFrm.swap(temp);

	m_pTexture = nullptr;
	m_CurIdx = 0;
}

void CAfterImageScript::SaveToFile(FILE* _File)
{
}

void CAfterImageScript::LoadFromFile(FILE* _File)
{
}

