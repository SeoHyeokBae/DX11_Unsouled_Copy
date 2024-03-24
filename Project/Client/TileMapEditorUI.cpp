#include "pch.h"
#include "TileMapEditorUI.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CTransform.h>
#include <Engine/CTileMap.h>
#include <Engine/CMeshRender.h>
#include <Engine/CTaskMgr.h>

#include <Scripts/CColTileScript.h>
#include <Scripts/CTypeTileScript.h>

// enum eTileType 변경시 수정필요
static string TYPE_STRING[(UINT)eTileType::END]
{
	"COLLIDER",
	"CLIFF",
	"WATER",
	"UPSTAIR",
	"DOWNSTAIR",
	"NONE",
};

TileMapEditorUI::TileMapEditorUI()
	: UI("Tile Editor", "##TileEditor")
	, m_CurSheet(nullptr)
	, m_DrawMode(TILE_DRAW_MODE::NONE)
	, m_Selected{}
	, m_bChange(false)
	, m_bCollider(false)
	
{	 
	Clear(1,1);
	Deactivate();
}

TileMapEditorUI::~TileMapEditorUI()
{
}

void TileMapEditorUI::render_update()
{
	static ImVec2 scrolling(0.0f, 0.0f);
	static float WheelSz = 1.f;
	static char tileMapName[256] = {};
	static UINT FaceX = 1;
	static UINT FaceY = 1;
	static Vector2 PixelSize = Vector2(32.f, 32.f);	// sheet 타일 사이즈
	static Vector2 Rendersize = Vector2(32.f, 32.f);	// Object 그려질 사이즈
	const char* combo_preview = "Select"; // combobox
	const char* idx_preview = "Select"; // combobox
	static int IsNotCollider = 11; // col일시 12 Create시 영향
	
	m_bCollider ? IsNotCollider = 12 : IsNotCollider = 11;

	if (ImGui::Button("Save"))
	{
		wchar_t szSelect[256] = { };
		OPENFILENAME ofn = {};

		char chName[256] = {};
		strcpy_s(chName, tileMapName);
		strncat_s(chName, ".tile", 6);
		CharToWChar(chName, szSelect);

		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = nullptr;
		ofn.lpstrFile = szSelect;
		ofn.nMaxFile = sizeof(szSelect);
		ofn.lpstrFilter = L"ALL\0*.*\0Tile\0*.tile";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;

		// 탐색창 초기 위치 지정
		wstring strInitPath = CPathMgr::GetContentPath();
		strInitPath += L"tile\\";
		ofn.lpstrInitialDir = strInitPath.c_str();

		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetSaveFileName(&ofn))
		{
			CTileMap* pTile = new CTileMap;
			pTile->SetFace(FaceX, FaceY);
			pTile->SetTileAtlas(m_CurSheet, PixelSize);
			pTile->SetTileInfoVec(m_vecTileInfo);

			// tile 을 저장할 경로
			wstring strTilePath = CPathMgr::GetContentPath();
			strTilePath += szSelect;

			FILE* pFile = nullptr;
			_wfopen_s(&pFile, CPathMgr::GetRelativePath(strTilePath).c_str(), L"wb");

			pTile->SaveToFile(pFile);
			
			fclose(pFile);
			delete pTile;
		}
	}	
	ImGui::SameLine();
	if (ImGui::Button("Load"))
	{
		wchar_t szSelect[256] = {};

		OPENFILENAME ofn = {};

		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = nullptr;
		ofn.lpstrFile = szSelect;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szSelect);
		ofn.lpstrFilter = L"ALL\0*.*\0Tile\0*.tile";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;

		// 탐색창 초기 위치 지정
		wstring strInitPath = CPathMgr::GetContentPath();
		strInitPath += L"tile\\";
		ofn.lpstrInitialDir = strInitPath.c_str();

		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetOpenFileName(&ofn))
		{
			// 이름 불러오기
			string name = ToString(CPathMgr::GetRelativePath(szSelect));
			name = name.substr(5); // "tile\\" 제거후 이름만 저장
			name.erase(name.end() - 5, name.end());
			strcpy(tileMapName, name.c_str());

			CTileMap* pTile = nullptr;

			// tile 을 불러올 경로
			wstring strTilePath = CPathMgr::GetContentPath();
			strTilePath += szSelect;

			FILE* pFile = nullptr;
			_wfopen_s(&pFile, CPathMgr::GetRelativePath(strTilePath).c_str(), L"rb");

			pTile = new CTileMap;
			pTile->LoadFromFile(pFile);

			FaceX = pTile->GetFaceX();
			FaceY = pTile->GetFaceY();
			Clear(FaceX, FaceY); // 타일 X * Y 그리드
			m_CurSheet = pTile->GetTileAtlas();
			m_vecTileInfo = pTile->GetInfoVec();
			PixelSize = pTile->GetTilePixelSize();

			ChangeSheet(m_CurSheet->GetWidth() / PixelSize.x, m_CurSheet->GetHeight() / PixelSize.y);
			//Rendersize = pTile->GetTilePixelSize();

			fclose(pFile);
			delete pTile;
		}

	}
	ImGui::SameLine();
	if (ImGui::Button("Create"))
	{
		CLevel* curlevel = CLevelMgr::GetInst()->GetCurrentLevel();
		CGameObject* tileObj = new CGameObject;
		tileObj->SetName(ToWString(tileMapName));
		tileObj->AddComponent(new CTransform);
		tileObj->AddComponent(new CTileMap);
		tileObj->AddComponent(new CCollider2D);
		tileObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
		tileObj->TileMap()->SetFace(FaceX, FaceY);
		tileObj->TileMap()->SetTileAtlas(m_CurSheet, PixelSize);
		tileObj->TileMap()->SetTileInfoVec(m_vecTileInfo);
		Vec2 colsize = Rendersize * Vec2(FaceX, FaceY);
		tileObj->Collider2D()->SetAbsolute(true);
		tileObj->Collider2D()->SetOffsetScale(colsize);
		tileObj->Collider2D()->SetVisible(true);
		// 충돌용) 속성 Script 생성 필요

		if(!m_bCollider)
			tileObj->AddComponent(new CTypeTileScript);

		GamePlayStatic::SpawnGameObject(tileObj, IsNotCollider); // 11 레이어 기본타일, 12레이어 콜라이더 타일
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

	// 오른쪽 마우스 드래그
	if (ImGui::IsWindowFocused() && is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right))
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
	ImGui::Text("Name"); ImGui::SameLine(79.f);
	ImGui::InputText("##TileMap", (char*)tileMapName, IM_ARRAYSIZE(tileMapName));
	static bool stepX = true;
	static bool stepY = true;
	const UINT UINT_Unit = 1;

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

				int row = m_CurSheet->GetHeight() / PixelSize.y;
				int col = m_CurSheet->GetWidth() / PixelSize.x;

				ChangeSheet(row, col);
			}

			// 리스트 중 해당 항목이 클릭되면 하이라이트 걸어줌
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	ImGui::Text("Pixel Size"); ImGui::SameLine();
	ImGui::DragFloat2("##Pixel Size", PixelSize);
	if (1 > PixelSize.x) PixelSize.x = 1;
	if (1 > PixelSize.y) PixelSize.y = 1;

	// 시트 번호선택
	if (nullptr != m_CurSheet)
	{
		ImGui::Text("Set Type"); ImGui::SameLine(86.f);
		ImGui::PushItemWidth(108);
		static int Sheet_idx = -1;
		vector<string> sheetidx;
		sheetidx.resize(m_vecType.size());
		for (size_t i = 0; i < sheetidx.size(); i++)
		{
			sheetidx[i] = std::to_string(i);;
		}

		if (-1 != Sheet_idx) idx_preview = sheetidx[Sheet_idx].c_str();
		if (ImGui::BeginCombo("##SelectSheetTile", idx_preview))
		{
			for (int n = 0; n < sheetidx.size(); n++)
			{
				const bool is_selected = (Sheet_idx == n);
				if (ImGui::Selectable(sheetidx[n].c_str(), is_selected))
				{
					Sheet_idx = n;
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::SameLine();
		// Sheet 선택후 -> 속성 선택
		const char* typeidx_preview = "<< Select";

		int type_idx = -1;
		if (-1 != Sheet_idx) 
		{
			type_idx = (UINT)m_vecType[Sheet_idx];
			typeidx_preview = TYPE_STRING[type_idx].c_str();
		}
		if (ImGui::BeginCombo("##SelectTileType", typeidx_preview))
		{
			for (int n = 0; n < (UINT)eTileType::END; n++)
			{
				const bool is_selected = (type_idx == n);
				if (ImGui::Selectable(TYPE_STRING[n].c_str(), is_selected))
				{
					type_idx = n;
					m_vecType[Sheet_idx] = (eTileType)type_idx;
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();
	}
	
	ImGui::Separator();
	ImGui::Text("Current Tile");ImGui::NewLine();

	//if (DrawModeEnum == (UINT)TILE_DRAW_MODE::NONE || DrawModeEnum == (UINT)TILE_DRAW_MODE::ERASER)
		//ImGui::Dummy(Vec2(50.f, 50.f));

	// Tile 
	if (nullptr != m_CurSheet)
	{
		ComPtr<ID3D11ShaderResourceView> my_texture = NULL;
		my_texture = m_CurSheet.Get()->GetSRV().Get();
		
		ImVec2 uvSize = PixelSize / ImVec2(m_CurSheet->GetWidth(), m_CurSheet->GetHeight());
		ImGui::Image((void*)my_texture.Get(), ImVec2(48.f, 48.f), m_Selected.vLeftTopUV, m_Selected.vLeftTopUV + uvSize);
		ImGui::SameLine(100.f);
		ImGui::Text("IsCollider");
		ImGui::SameLine();
		ImGui::Checkbox("##IsCollider", &m_bCollider);
		ImGui::NewLine(); ImGui::Separator();

		float fwidth = m_CurSheet->GetWidth();
		float fheight = m_CurSheet->GetHeight();
		int row = fheight / PixelSize.y;
		int col = fwidth / PixelSize.x;

		int count = 0;
		for (size_t i = 0; i < row; i++)
		{
			for (size_t j = 0; j < col; j++)
			{
				count++;
				ImVec2 uvLT = ImVec2(PixelSize.x * j / fwidth, PixelSize.y * i / fheight);
				ImVec2 uvRB = ImVec2(PixelSize.x * (j + 1) / fwidth, PixelSize.y * (i + 1) / fheight);
				char _id[256] = {};
				sprintf_s(_id, "_id%d", count);
				
				if (ImGui::ImageButton(_id, (void*)my_texture.Get(), ImVec2(48.f, 48.f), uvLT, uvRB))
				{
					m_Selected.vLeftTopUV = uvLT;
					m_Selected.eType = m_vecType[i * col + j ];
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
			if (rect.Contains(io.MousePos) && ImGui::IsWindowFocused() && (ImGui::IsMouseClicked(ImGuiMouseButton_Left)|| ImGui::IsMouseDown(ImGuiMouseButton_Left)))
			{
				ClickEvent(FaceX, FaceY, i, j);
			}

			UINT idx = i * FaceX + j;
			if (m_vecTileInfo.size() !=0 && m_vecTileInfo[idx].bRender)
			{
				ImVec2 size = ImVec2(PixelSize.x / m_CurSheet->GetWidth(), PixelSize.y / m_CurSheet->GetHeight());
				draw_list->AddImage((void*)m_CurSheet.Get()->GetSRV().Get(), LT, RB, m_vecTileInfo[idx].vLeftTopUV, m_vecTileInfo[idx].vLeftTopUV + size);
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
		m_vecTileInfo[idx].vLeftTopUV = m_Selected.vLeftTopUV;
		m_vecTileInfo[idx].eType = m_Selected.eType;
		m_vecTileInfo[idx].bRender = true;
	}
		break;
	case TILE_DRAW_MODE::FILL:
		for (size_t i = 0; i < _faceY; i++)
		{
			for (size_t j = 0; j < _faceX; j++)
			{
				UINT idx = i * _faceX + j;
				if (!m_vecTileInfo[idx].bRender)
				{
					m_vecTileInfo[idx].vLeftTopUV = m_Selected.vLeftTopUV;
					m_vecTileInfo[idx].eType = m_Selected.eType;
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
	m_vecTileInfo.clear();
	vector<tTileInfo> vecTemp;
	m_vecTileInfo.swap(vecTemp);
	m_vecTileInfo.resize(_faceX * faceY);
}

// Sheet 변경 시 속성 초기화
void TileMapEditorUI::ChangeSheet(int _row, int _col)
{
	m_vecType.clear();
	vector<eTileType> vecTemp;
	m_vecType.swap(vecTemp);
	m_vecType.resize(_row * _col, eTileType::NONE);
}

void TileMapEditorUI::Deactivate()
{   
	UI::Deactivate();
}


