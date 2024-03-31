#include "pch.h"
#include "CAfterImageScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
//#include <Engine/CGameObject.h>

CAfterImageScript::CAfterImageScript()
	: CScript(AFTERIMAGESCRIPT)
	, m_AfterImgObj(nullptr)
	, m_CurIdx(0)
	, m_FadeSpeed(10.f)

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
	for (int i = 0; i < IMGCOUNT; i++)
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

		m_vSprite.push_back(sprite);
		m_AfterImgObj->AddChild(m_vSprite[i]);
		GamePlayStatic::SpawnGameObject(m_vSprite[i], 11); // 추후 변경

		mbool.push_back(false);
	}
	


}

void CAfterImageScript::tick()
{
	Vec3 vOwnerPos = Transform()->GetRelativePos();
	m_AfterImgObj->Transform()->SetRelativePos(-vOwnerPos);
	
	// 이미지 업데이트 
	if (GetOwner()->IsAfterImgAct())
	{
		Update();
	}

}


void CAfterImageScript::Update()
{
	for (size_t i = 0; i < m_qFrm.size(); i++)
	{
		//if (mbool[i])
		//{
			m_qFrm[i].fAlhpa -= DT ;
			Vec4 vColor = Vec4(0.0f, 0.0f, 0.0f, m_qFrm[i].fAlhpa);
			m_vSprite[i]->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, vColor); //색상
			//return;
		//}

		mbool[i] = true;

		Vec3 vPos = m_qFrm[i].vPos;
		Vec2 vLT = m_qFrm[i].AnimFrm.vLeftTop;
		Vec2 vBg = m_qFrm[i].AnimFrm.vBackground;
		Vec2 vSize = m_qFrm[i].AnimFrm.vSlice;


		m_vSprite[i]->Transform()->SetRelativePos(vPos);
		m_vSprite[i]->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_1, m_pTexture);
		m_vSprite[i]->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_0, vLT); //UV
		m_vSprite[i]->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_1, vBg); //Bg
		m_vSprite[i]->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_2, vSize); //Size
	}

}

void CAfterImageScript::Clear()
{
	m_qFrm.clear();
	vector<tAftInfo> temp;
	m_qFrm.swap(temp);

	m_pTexture = nullptr;
	m_CurIdx = 0;
}

void CAfterImageScript::SaveToFile(FILE* _File)
{
}

void CAfterImageScript::LoadFromFile(FILE* _File)
{
}

