#include "pch.h"
#include "MaterialUI.h"

#include <Engine/CAssetMgr.h>
#include <Engine/CMaterial.h>
#include <Engine/CGraphicsShader.h>
#include <Engine/CTexture.h>

#include "ParamUI.h"

// SAVE 시 PARAM 하드코딩 수정 필요
// PARAM 도 선택후 넣기 가능하게

MaterialUI::MaterialUI()
	: AssetUI("Material", "##Material", ASSET_TYPE::MATERIAL)
{
}

MaterialUI::~MaterialUI()
{
}


void MaterialUI::render_update()
{
	AssetUI::render_update();


    // 해당 텍스쳐 이미지 출력
    Ptr<CMaterial> pMtrl = (CMaterial*)GetAsset().Get();
    string strPath = string(pMtrl->GetRelativePath().begin(), pMtrl->GetRelativePath().end());
    static char sName[256] = {};

    ImGui::Text((char*)strPath.c_str());
    ImGui::Separator();
    ImGui::Text("NewName ");
    ImGui::SameLine();
    ImGui::InputText("##InputTexName", (char*)sName, IM_ARRAYSIZE(sName));

    Ptr<CGraphicsShader> pShader = pMtrl->GetShader();
    string strShaderName;
    if (nullptr != pShader)
    {
        strShaderName = ToString(pShader->GetKey());
    }
    ImGui::Text("Shader  ");
    ImGui::SameLine();
    ImGui::InputText("##ShaderName", (char*)strShaderName.c_str(), strShaderName.length(), ImGuiInputTextFlags_ReadOnly);
    // Shader Drop 체크
    if (ImGui::BeginDragDropTarget())
    {
        const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");

        if (payload)
        {
            DWORD_PTR data = *((DWORD_PTR*)payload->Data);
            CAsset* pAsset = (CAsset*)data;
            if (ASSET_TYPE::GRAPHICS_SHADER == pAsset->GetType())
            {
                pMtrl->SetShader((CGraphicsShader*)pAsset);
            }
        }
        ImGui::EndDragDropTarget();
    }

    ImGui::Separator();
    if (ImGui::Button("Save"))
    {
        wchar_t szPath[255] = {};
        wstring FilePath = CPathMgr::GetContentPath();
        CMaterial* mtrl = new CMaterial;
        if ('\0' == sName[0])
        {
            int num = 0;
            while (true)
            {
                swprintf_s(szPath, L"Material\\New_Edit_Material_%d.mtrl", num);
                if (!exists(FilePath + szPath))
                    break;
                ++num;
            }
            mtrl->SetName(szPath);
            mtrl->Save(szPath);
        }
        else
        {
            string str = (string)sName;
            FilePath += L"Material\\";
            FilePath += ToWString(str) + L".mtrl";

            mtrl->SetName(CPathMgr::GetRelativePath(FilePath));
            mtrl->SetTexParam(TEX_PARAM::TEX_0, pMtrl->GetTexParam(TEX_PARAM::TEX_0));
            mtrl->SetScalarParam(SCALAR_PARAM::INT_0, pMtrl->GetScalarParam(SCALAR_PARAM::INT_0));
            mtrl->SetShader(pMtrl->GetShader());
            mtrl->Save(CPathMgr::GetRelativePath(FilePath));
            sName[0] = {0};
        }

         GamePlayStatic::AddAsset(mtrl);
    }
    ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
    ImGui::Text("Material Parameter");
    ImGui::Spacing(); ImGui::Spacing();

    // Shader Parameter
    if (nullptr == pShader)
        return;

    const vector<tScalarParam>& vecScalarParam = pShader->GetScalarParam();
    for (size_t i = 0; i < vecScalarParam.size(); ++i)
    {
        switch (vecScalarParam[i].Type)
        {
        case SCALAR_PARAM::INT_0:
        case SCALAR_PARAM::INT_1:
        case SCALAR_PARAM::INT_2:
        case SCALAR_PARAM::INT_3:
            ParamUI::Param_INT((int*)pMtrl->GetScalarParam(vecScalarParam[i].Type), vecScalarParam[i].Desc);
            break;
        break;
        case SCALAR_PARAM::FLOAT_0:
        case SCALAR_PARAM::FLOAT_1:
        case SCALAR_PARAM::FLOAT_2:
        case SCALAR_PARAM::FLOAT_3:
            ParamUI::Param_FLOAT((float*)pMtrl->GetScalarParam(vecScalarParam[i].Type), vecScalarParam[i].Desc);
            break;
        case SCALAR_PARAM::VEC2_0:
        case SCALAR_PARAM::VEC2_1:
        case SCALAR_PARAM::VEC2_2:
        case SCALAR_PARAM::VEC2_3:
            ParamUI::Param_VEC2((Vec2*)pMtrl->GetScalarParam(vecScalarParam[i].Type), vecScalarParam[i].Desc);
            break;
        case SCALAR_PARAM::VEC4_0:
        case SCALAR_PARAM::VEC4_1:
        case SCALAR_PARAM::VEC4_2:
        case SCALAR_PARAM::VEC4_3:
            ParamUI::Param_VEC4((Vec4*)pMtrl->GetScalarParam(vecScalarParam[i].Type), vecScalarParam[i].Desc);
            break;
        case SCALAR_PARAM::MAT_0:
        case SCALAR_PARAM::MAT_1:
        case SCALAR_PARAM::MAT_2:
        case SCALAR_PARAM::MAT_3:
            break;
        }
    }

    const vector<tTexParam>& vecTexParam = pShader->GetTexParam();
    for (size_t i = 0; i < vecTexParam.size(); i++)
    {
        Ptr<CTexture> pTex = pMtrl->GetTexParam(vecTexParam[i].Type);
        if (ParamUI::Param_TEXTURE(pTex, vecTexParam[i].Desc, this, (Delegate_1)&MaterialUI::SelectTexture))
        {
            // 리스트 버튼을 눌렀다면
            m_SelectTexParam = vecTexParam[i].Type;
        }
        pMtrl->SetTexParam(vecTexParam[i].Type, pTex);
    }


}

void MaterialUI::SelectTexture(DWORD_PTR _dwData)
{
    string strTex = (char*)_dwData;
    wstring strTexName = ToWString(strTex);

    Ptr<CTexture> pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(strTexName);
    Ptr<CMaterial> pMtrl = (CMaterial*)GetAsset().Get();
    pMtrl->SetTexParam(m_SelectTexParam, pTex);
}
