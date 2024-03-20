#include "pch.h"
#include "TileMapEditorUI.h"


TileMapEditorUI::TileMapEditorUI()
	: UI("Tile Editor", "##TileEditor")
	, m_CurSheet(nullptr)
	, m_DrawMode(TILE_DRAW_MODE::NONE)
	, m_Selected{}
	, m_bChange(false)
{
	Deactivate();
}

TileMapEditorUI::~TileMapEditorUI()
{
}

void TileMapEditorUI::render_update()
{
	static ImVec2 scrolling(0.0f, 0.0f);
	static float WheelSz = 1.f;

	if (ImGui::Button("Create"))
	{
	}
	ImGui::SameLine();
	if (ImGui::Button("Exit"))
	{
		UI::Deactivate();
	}
	ImGui::SameLine();
	ImGui::Text("Mouse wheel: %.1f", WheelSz);

	ImGui::Separator();

	// Draw Canvas
	ImVec2 canvas_LT = ImGui::GetCursorScreenPos();
	ImVec2 canvas_SZ = ImGui::GetContentRegionAvail();
	if (canvas_SZ.x < 50.0f) canvas_SZ.x = 50.0f; 
	if (canvas_SZ.y < 50.0f) canvas_SZ.y = 50.0f;
	ImVec2 canvas_RB = ImVec2(canvas_LT.x + canvas_SZ.x, canvas_LT.y + canvas_SZ.y);
	ImGuiIO& io = ImGui::GetIO();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->AddRectFilled(canvas_LT, canvas_RB, IM_COL32(50, 50, 50, 255));

	// This will catch our interactions
	ImGui::InvisibleButton("canvas", canvas_SZ, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
	const bool is_hovered = ImGui::IsItemHovered(); // Hovered
	const bool is_active = ImGui::IsItemActive();   // Held

	ImVec2 CenterPos = canvas_SZ / 2.f;
	CenterPos -= scrolling;
	ImVec2 WheelOffset = CenterPos * WheelSz - CenterPos;
	const ImVec2 origin(canvas_LT.x + scrolling.x - WheelOffset.x, canvas_LT.y + scrolling.y - WheelOffset.y);
	const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);

	// 좌클릭시
	if (ImGui::IsWindowFocused() && m_DrawMode != TILE_DRAW_MODE::NONE && is_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	{
		//cutting_rect = true;

		UINT test = 0;
		switch (test)
		{
		case (UINT)TILE_DRAW_MODE::PAINT:
			break;
		case (UINT)TILE_DRAW_MODE::FILL:
			break;
		case (UINT)TILE_DRAW_MODE::ERASER:
			break;
		case (UINT)TILE_DRAW_MODE::NONE:
			break;
		default:
			break;
		}
	}
	//if (cutting_rect)
	//{
	//	points.back() = mouse_pos_in_canvas / WheelSz;

	//	if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
	//	{
	//		cutting_rect = false;
	//		ImRect rect(points[points.size() - 2], points.back());
	//		if (rect.Min.x > rect.Max.x) ImSwap(rect.Min.x, rect.Max.x);
	//		if (rect.Min.y > rect.Max.y) ImSwap(rect.Min.y, rect.Max.y);
	//		m_vecRect.push_back(rect);
	//		m_bSlice = false;
	//	}
	//}

	// 오른쪽 마우스 드래그
	//static bool opt_enable_context_menu = true;
	//const float mouse_threshold_for_pan = opt_enable_context_menu ? -1.0f : 0.0f;
	if (ImGui::IsWindowFocused() && is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right)/*, opt_enable_context_menu*/)
	{
		scrolling.x += io.MouseDelta.x;
		scrolling.y += io.MouseDelta.y;
	}

	// 마우스 휠
	if (ImGui::IsWindowFocused() && (io.MouseWheel > 0.f || io.MouseWheel))
	{
		ImRect InCanvas(canvas_LT, canvas_RB);
		if (InCanvas.Contains(io.MousePos))
		{
			WheelSz += 0.075f * io.MouseWheel;
			if (WheelSz <= 0.f) WheelSz = 0.f;
		}
	}

	// Info
	ImGui::Begin("Tile Info");
	ImGui::BeginChild("child", ImGui::GetContentRegionAvail(), ImGuiChildFlags_Border);
	static char tileMapName[256] = {};
	ImGui::Text("Name"); ImGui::SameLine(79.f);
	ImGui::InputText("##TileMap", (char*)tileMapName, IM_ARRAYSIZE(tileMapName));
	static bool stepX = true;
	static bool stepY = true;
	const UINT UINT_Unit = 1;
	static UINT FaceX = 1;
	static UINT FaceY = 1;

	UINT prvFaceX = FaceX;
	UINT prvFaceY = FaceY;

	ImGui::Text("Face X"); ImGui::SameLine(79.f);
	ImGui::InputScalar("##FaceX", ImGuiDataType_U32, &FaceX, stepX ? &UINT_Unit : NULL, NULL);
	ImGui::Text("Face Y"); ImGui::SameLine(79.f);
	ImGui::InputScalar("##FaceY", ImGuiDataType_U32, &FaceY, stepY ? &UINT_Unit : NULL, NULL);

	if (prvFaceX != FaceX || prvFaceY != FaceY)	
		m_bChange = true;


	if (m_bChange)
	{
		m_bChange = false;
		Clear(FaceX, FaceY);
	}


	ImGui::Text("Tile Size"); ImGui::SameLine();
	static Vector2 Rendersize = Vector2(64.f, 64.f);	// ObjectSize
	ImGui::DragFloat2("##Tile Render Size", Rendersize);

	ImGui::Text("Mode  "); ImGui::SameLine();
	static int DrawModeEnum = (UINT)m_DrawMode;
	if (ImGui::RadioButton("None", &DrawModeEnum, 3))
		m_DrawMode = TILE_DRAW_MODE::NONE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Paint", &DrawModeEnum, 0))
		m_DrawMode = TILE_DRAW_MODE::PAINT;
	ImGui::SameLine();
	if (ImGui::RadioButton("Fill", &DrawModeEnum, 1))
		m_DrawMode = TILE_DRAW_MODE::FILL;
	ImGui::SameLine();
	if (ImGui::RadioButton("Eraser", &DrawModeEnum, 2))
		m_DrawMode = TILE_DRAW_MODE::ERASER;

	ImGui::Separator();

	// Select Atlas
	ImGui::Text("Tile Sheet"); ImGui::SameLine();
	static int Atlas_idx = -1;
	const map<wstring, Ptr<CAsset>>& mapAsset = CAssetMgr::GetInst()->GetAssets(ASSET_TYPE::TEXTURE);
	vector<string> atlas;

	CAssetMgr::GetInst()->GetAssetName(ASSET_TYPE::TEXTURE, atlas);

	const char* combo_preview = "Select";
	if (-1 != Atlas_idx) combo_preview = atlas[Atlas_idx].c_str();
	if (ImGui::BeginCombo("##SelectTileAtlas", combo_preview))
	{
		for (int n = 0; n < atlas.size(); n++)
		{
			const bool is_selected = (Atlas_idx == n);
			if (ImGui::Selectable(atlas[n].c_str(), is_selected))
			{
				Atlas_idx = n;
				Ptr<CTexture> Tex = CAssetMgr::GetInst()->FindAsset<CTexture>(ToWString(atlas[n]));
				m_CurSheet = Tex;
			}

			// 리스트 중 해당 항목이 클릭되면 하이라이트 걸어줌
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	ImGui::Text("Pixel Size"); ImGui::SameLine();
	static Vector2 PixelSize = Vector2(64.f, 64.f);	// ObjectSize
	ImGui::DragFloat2("##Pixel Size", PixelSize);
	if (1 > PixelSize.x) PixelSize.x = 1;
	if (1 > PixelSize.y) PixelSize.y = 1;
	ImGui::Separator();

	ImGui::Text("Current Tile");ImGui::NewLine();

	//if (DrawModeEnum == (UINT)TILE_DRAW_MODE::NONE || DrawModeEnum == (UINT)TILE_DRAW_MODE::ERASER)
		//ImGui::Dummy(Vec2(50.f, 50.f));

	// Tile 
	if (nullptr != m_CurSheet)
	{
		ComPtr<ID3D11ShaderResourceView> my_texture = NULL;
		my_texture = m_CurSheet.Get()->GetSRV().Get();
		ImGui::Image((void*)my_texture.Get(), ImVec2(48.f, 48.f), m_Selected.Min, m_Selected.Max);

		ImGui::NewLine(); ImGui::Separator();

		float fwidth = m_CurSheet->GetWidth();
		float fheight = m_CurSheet->GetHeight();

		int count = 0;
		for (size_t i = 0; i < fheight / PixelSize.y; i++)
		{
			for (size_t j = 0; j < fwidth / PixelSize.x; j++)
			{
				count++;
				ImVec2 uvLT = ImVec2(PixelSize.x * j / fwidth, PixelSize.y * i / fheight);
				ImVec2 uvRB = ImVec2(PixelSize.x * (j + 1) / fwidth, PixelSize.y * (i + 1) / fheight);
				char _id[256] = {};
				sprintf_s(_id, "_id%d", count);
				
				if (ImGui::ImageButton(_id, (void*)my_texture.Get(), ImVec2(48.f, 48.f), uvLT, uvRB))
				{
					m_Selected = ImRect(uvLT, uvRB);
				}

				if (0 == count % 5)
					continue;

				ImGui::SameLine();
			}
		}
	}
	ImGui::EndChild();
	ImGui::End();
	//FaceX , FaceY
	draw_list->PushClipRect(canvas_LT, canvas_RB, true);
	ImVec2 left_top = canvas_LT + scrolling - WheelOffset;
	ImVec2 new_render_size =  Rendersize * WheelSz;

	// 타일 렌더링
	for (size_t i = 0; i < FaceY; i++)
	{
		for (size_t j = 0; j < FaceX; j++)
		{
			ImVec2 LT = ImVec2(left_top.x + new_render_size.x * j, left_top.y + new_render_size.y * i);
			ImVec2 RB = ImVec2(left_top.x + new_render_size.x * (j + 1), left_top.y + new_render_size.y * (i + 1));
			draw_list->AddRect(LT, RB, IM_COL32(255, 255, 255, 255));

			ImRect rect = ImRect(LT, RB);
			if (rect.Contains(io.MousePos) && ImGui::IsWindowFocused() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			{
				ClickEvent(FaceX, FaceY, i, j);
			}

			UINT idx = i * FaceX + j;
			if (m_vecTileInfo.size() !=0 && m_vecTileInfo[idx].bRender)
			{
				ImVec2 size = m_Selected.Max - m_Selected.Min;
				draw_list->AddImage((void*)m_CurSheet.Get()->GetSRV().Get(), LT, RB, m_vecTileInfo[idx].vLeftTopUV, m_vecTileInfo[idx].vLeftTopUV+ size);
			}
		}
	}
	draw_list->PopClipRect();
}

void TileMapEditorUI::ClickEvent(int _faceX, int _faceY, int _row, int _col)
{
	if (nullptr == m_CurSheet)
		return;

	UINT idx = _row * _faceX + _col;
	switch (m_DrawMode)
	{
	case TILE_DRAW_MODE::PAINT:
	{
		ImVec2 uvlt = m_Selected.Min;
		m_vecTileInfo[idx].vLeftTopUV = uvlt;
		m_vecTileInfo[idx].bRender = 1;
	}
		break;
	case TILE_DRAW_MODE::FILL:
		for (size_t i = 0; i < _faceX; i++)
		{
			for (size_t j = 0; j < _faceY; j++)
			{
				UINT idx = i * _faceX + j;
				if (!m_vecTileInfo[idx].bRender)
				{
					ImVec2 uvlt = m_Selected.Min;
					m_vecTileInfo[idx].vLeftTopUV = uvlt;
					m_vecTileInfo[idx].bRender = true;
				}
			}
		}
		break;
	case TILE_DRAW_MODE::ERASER:
		m_vecTileInfo[idx] = {};
		break;
	case TILE_DRAW_MODE::NONE:
		break;
	}
}

void TileMapEditorUI::Clear(int _faceX, int faceY)
{
	if (0 != m_vecTileInfo.size())
	{
		m_vecTileInfo.clear();
	}
	vector<tTileInfo> vecTemp;
	m_vecTileInfo.swap(vecTemp);
	m_vecTileInfo.resize(_faceX * faceY);
}

void TileMapEditorUI::Deactivate()
{   
	UI::Deactivate();
}


