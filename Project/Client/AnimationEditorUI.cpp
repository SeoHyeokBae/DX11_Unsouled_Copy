#include "pch.h"
#include "AnimationEditorUI.h"

#include <Engine/CTexture.h>
#include <Engine/CAssetMgr.h>

AnimationEditorUI::AnimationEditorUI()
	: UI("Animation Editor", "##AnimationEditor")
{
	Deactivate();
}

AnimationEditorUI::~AnimationEditorUI()
{
}

void AnimationEditorUI::render_update()
{

	//ID3D11ShaderResourceView* my_texture = NULL;
	ComPtr<ID3D11ShaderResourceView> my_texture = NULL;
	Ptr<CTexture> pAltasTex = CAssetMgr::GetInst()->Load<CTexture>(L"AnimAtlasTex", L"texture\\link.png");
	my_texture = pAltasTex.Get()->GetSRV().Get();
	float my_image_width = pAltasTex.Get()->GetWidth() * 0.3f;
	float my_image_height = pAltasTex.Get()->GetHeight() * 0.3f;


	ImGui::Begin("DirectX11 Texture Test");
	ImGui::Text("pointer = %p", my_texture);
	ImGui::Text("size = %d x %d", my_image_width, my_image_height);
	ImGui::Image((void*)my_texture.Get(), ImVec2(my_image_width, my_image_height));
	ImGui::End();
}

void AnimationEditorUI::Deactivate()
{
	UI::Deactivate();
}

