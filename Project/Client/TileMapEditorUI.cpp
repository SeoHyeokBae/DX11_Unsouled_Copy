#include "pch.h"
#include "TileMapEditorUI.h"


TileMapEditorUI::TileMapEditorUI()
	: UI("Tile Editor", "##TileEditor")
	, m_CurAtlas(nullptr)
{
	Deactivate();
}

TileMapEditorUI::~TileMapEditorUI()
{
}
void TileMapEditorUI::render_update()
{
	if (ImGui::Button("Create"))
	{
	}
	ImGui::SameLine();
	if (ImGui::Button("Exit"))
	{
	}
	ImGui::Separator();

	// Draw Canvas
	static ImVec2 scrolling(0.0f, 0.0f);
	static float WheelSz = 1.f;
	ImVec2 canvas_LT = ImGui::GetCursorScreenPos();
	ImVec2 canvas_SZ = ImGui::GetContentRegionAvail();
	if (canvas_SZ.x < 500.0f) canvas_SZ.x = 50.0f; 
	if (canvas_SZ.y < 50.0f) canvas_SZ.y = 50.0f;
	ImVec2 canvas_RB = ImVec2(canvas_LT.x + canvas_SZ.x, canvas_LT.y + canvas_SZ.y);
	ImGuiIO& io = ImGui::GetIO();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->AddRectFilled(canvas_LT, canvas_RB, IM_COL32(50, 50, 50, 255));


	// Info
	ImGui::Begin("Tile Info");
	ImGui::Text("Tile Atlas"); ImGui::SameLine();
	
	// Select Atlas
	static int Atlas_idx = -1;
	const map<wstring, Ptr<CAsset>>& mapAsset = CAssetMgr::GetInst()->GetAssets(ASSET_TYPE::TEXTURE);
	vector<string> atlas;

	CAssetMgr::GetInst()->GetAssetName(ASSET_TYPE::TEXTURE, atlas);

	const char* combo_preview = "Select";
	if(-1 != Atlas_idx) combo_preview = atlas[Atlas_idx].c_str();
	if (ImGui::BeginCombo("##SelectTileAtlas", combo_preview))
	{
		for (int n = 0; n < atlas.size(); n++)
		{
			const bool is_selected = (Atlas_idx == n);
			if (ImGui::Selectable(atlas[n].c_str(), is_selected))
			{
				Atlas_idx = n;
				Ptr<CTexture> Tex = CAssetMgr::GetInst()->FindAsset<CTexture>(ToWString(atlas[n]));
				m_CurAtlas = Tex;
			}

			// 리스트 중 해당 항목이 클릭되면 하이라이트 걸어줌
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	ImGui::BeginChild("child", ImGui::GetContentRegionAvail(), ImGuiChildFlags_Border);
	ImGui::EndChild();
	ImGui::End();

}

void TileMapEditorUI::Deactivate()
{
	UI::Deactivate();
}
