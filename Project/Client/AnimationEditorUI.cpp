#include "pch.h"
#include "AnimationEditorUI.h"

#include <Engine/CPathMgr.h>
#include <Engine/CAssetMgr.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CTimeMgr.h>

#include "ListUI.h"

AnimationEditorUI::AnimationEditorUI()
	: UI("Animation Editor", "##AnimationEditor")
	, m_CurAtlas(nullptr)
	, m_CanvasLeftTop(ImVec2(0.f,0.f))
	, m_Scrolling(ImVec2(0.f,0.f))
	, m_MousePos(ImVec2(0.f,0.f))
	, m_CenterPos(ImVec2(0.f,0.f))
	, m_CanvasIdx(0)
	, m_AnimIdx(-1)
	, m_fps(1.f)
	, m_Wheelsz(1.f)
	, m_bSlice(false)
	, m_bTrim(false)
	, m_bSmartSlice(false)
	, m_FrmInfo{}
{
	Deactivate();
}

AnimationEditorUI::~AnimationEditorUI()
{
}

void AnimationEditorUI::render_update()
{
	// botton 추가
	if (ImGui::Button("Load Texture"))
	{
		ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("Select##List");

		const map<wstring, Ptr<CAsset>>& mapAsset = CAssetMgr::GetInst()->GetAssets(ASSET_TYPE::TEXTURE);
		for (const auto& pair : mapAsset)
			pListUI->AddString(string(pair.first.begin(), pair.first.end()));

		pListUI->SetDbClickDelegate(this, (Delegate_1)&AnimationEditorUI::SelectAtlas);
		pListUI->Activate();
	}
	ImGui::SameLine();
	if (ImGui::Button("Slice Sprite"))
	{
		m_bSlice = true;
		m_bTrim = false;
		m_bSmartSlice = false;
	}
	ImGui::SameLine();
	if (ImGui::Button("Trim Slice"))
	{
		m_bTrim = true;
		m_bSlice = false;
		m_bSmartSlice = false;
	}
	ImGui::SameLine();
	if (ImGui::Button("Smart Slice"))
	{
		m_bSmartSlice = true;
		m_bSlice = false;
		m_bTrim = false;
	}
	ImGui::SameLine();
	if (ImGui::Button("Add Sprite to Animation"))
	{
		tAnimFrm frm = {};
		frm.vLeftTop = m_vecRect[m_CanvasIdx].Min;
		frm.vSlice = m_vecRect[m_CanvasIdx].GetSize();
		m_vecAnimRect.push_back(frm);
		m_AnimIdx = m_vecAnimRect.size() - 1;
	}

	ImGuiIO& io = ImGui::GetIO();

	ImGui::Text("Mouse pos: (%g, %g)", m_MousePos.x, m_MousePos.y);
	ImGui::Text("Center pos: (%g, %g)", m_CenterPos.x, m_CenterPos.y);
	ImGui::Text("Mouse wheel: %.1f", m_Wheelsz);
	DrawCanvas();


	// animation make
	ImGui::Begin("Sprite Animation");
	if (ImGui::Button("New Animation"))
	{
		// todo
	}
	ImGui::SameLine();
	if (ImGui::Button("Load Animation"))
	{
		// todo
	}
	ImGui::SameLine();
	if (ImGui::Button("Save Animation"))
	{
		assert(!(0 != m_vecAnimRect.size()));
		//// 애니메이션 이름 저장
		//SaveWString(GetName(), _File);

		//// 모든 프레임 정보 저장
		//size_t FrmSize = m_vecFrm.size();
		//fwrite(&FrmSize, sizeof(size_t), 1, _File);
		//fwrite(m_vecFrm.data(), sizeof(tAnimFrm), m_vecFrm.size(), _File);

		//// 애니메이션이 참조하던 텍스쳐 정보 저장
		//SaveAssetRef(m_AtlasTex, _File);

		//vector<tAnimFrm> vAnim;
		//Vec2 bgsize = Vec2(0.f,0.f);
		//for (size_t i = 0; i < m_vecAnimRect.size(); i++)
		//{
		//	if (bgsize.x < m_vecAnimRect[i].GetWidth()) bgsize.x = m_vecAnimRect[i].GetWidth();
		//	if (bgsize.y < m_vecAnimRect[i].GetHeight()) bgsize.y = m_vecAnimRect[i].GetHeight();
		//	tAnimFrm frm = {};
		//	frm.vLeftTop = m_vecAnimRect[i].Min;
		//	frm.vSlice = m_vecAnimRect[i].GetSize();
		//}
		//bgsize += 200.f;

	}

	// preview
	ImVec2 PrevCanvasLT = ImGui::GetCursorScreenPos();    
	ImVec2 Prevcanvas_sz = ImVec2(250.f,250.f);
	ImVec2 PrevCanvasRB = PrevCanvasLT + Prevcanvas_sz;
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->AddRectFilled(PrevCanvasLT, PrevCanvasRB, IM_COL32(70, 70, 70, 255));
	draw_list->AddRectFilled(PrevCanvasLT, PrevCanvasRB - Prevcanvas_sz / 2, IM_COL32(50, 50, 50, 255));
	draw_list->AddRectFilled(PrevCanvasLT + Prevcanvas_sz /2, PrevCanvasRB, IM_COL32(50, 50, 50, 255));
	if (0 != m_vecAnimRect.size())
	{
		// Canvas 안에 이미지출력
		draw_list->PushClipRect(PrevCanvasLT, PrevCanvasRB, true);

		ImVec2 displayLT = ImVec2(m_vecAnimRect[m_AnimIdx].vLeftTop);
		ImVec2 displayRB = ImVec2(m_vecAnimRect[m_AnimIdx].vLeftTop + m_vecAnimRect[m_AnimIdx].vSlice);
		ImVec2 displaySize = ImVec2(m_vecAnimRect[m_AnimIdx].vSlice);
		UINT texturewidth = (m_CurAtlas.Get()->GetWidth());
		UINT textureheight = (m_CurAtlas.Get()->GetHeight());

		ImVec2 uv0 = ImVec2(displayLT.x / texturewidth, displayLT.y / textureheight);
		ImVec2 uv1 = ImVec2((displayLT.x + displaySize.x) / texturewidth, (displayLT.y + displaySize.y) / textureheight);

		ImRect rec(PrevCanvasLT, PrevCanvasRB);
		rec.Expand(-50.f);
		rec.Translate(m_vecAnimRect[m_AnimIdx].vOffset);
		draw_list->AddImage(m_CurAtlas.Get()->GetSRV().Get(), rec.Min, rec.Max, uv0, uv1);
		draw_list->PopClipRect();
	}

	// 십자선
	draw_list->AddLine(ImVec2(PrevCanvasLT.x + Prevcanvas_sz.x/2, PrevCanvasLT.y)
					 , ImVec2(PrevCanvasLT.x + Prevcanvas_sz.x/2, PrevCanvasRB.y), IM_COL32(0, 255, 0, 150));
	draw_list->AddLine(ImVec2(PrevCanvasLT.x, PrevCanvasLT.y + Prevcanvas_sz.y/2)
					 , ImVec2(PrevCanvasRB.x, PrevCanvasLT.y + Prevcanvas_sz.y/2), IM_COL32(255, 0, 0, 150));

	//Anim 정보
	static string spritename;
	ImGui::SetCursorPosX(265.f);
	ImGui::Text("Name"); 
	ImGui::SetCursorPosX(265.f);
	ImGui::InputText("##Sprite", (char*)spritename.c_str(), spritename.length()); 
	ImGui::SetCursorPosX(265.f); ImGui::Separator();

	ImGui::SetCursorPos(ImVec2(265.f,110.f));
	ImGui::Text("Frame Number");
	ImGui::SetCursorPosX(265.f);
	static bool inputs_step = true;
	const int frmUnit = 1;
	ImGui::PushItemWidth(125);
	ImGui::InputScalar("##FrameNum", ImGuiDataType_U8, &m_AnimIdx, inputs_step ? &frmUnit : NULL, NULL, "%u");
	if (m_AnimIdx > m_vecAnimRect.size() - 1) m_AnimIdx = m_vecAnimRect.size() - 1;
	ImGui::SetCursorPosX(265.f); ImGui::Separator();
	ImGui::PopItemWidth();

	ImGui::SetCursorPos(ImVec2(265.f, 180.f));
	ImVec2 offset = ImVec2(0.f, 0.f);
	if(0 != m_vecAnimRect.size()) offset = ImVec2(m_vecAnimRect[m_AnimIdx].vOffset);
	ImGui::Text("Offset : X = %0.f, Y = %0.f", offset.x, offset.y);

	ImGui::SetCursorPos(ImVec2(335.f,212.f));
	if (ImGui::ArrowButton("##Up", ImGuiDir_Up)) { m_vecAnimRect[m_AnimIdx].vOffset.y--; }
	ImGui::SetCursorPosX(313.f);
	if (ImGui::ArrowButton("##left", ImGuiDir_Left)) { m_vecAnimRect[m_AnimIdx].vOffset.x--; }
	ImGui::SameLine(); ImGui::SetCursorPosX(357.f);
	if (ImGui::ArrowButton("##Right", ImGuiDir_Right)) { m_vecAnimRect[m_AnimIdx].vOffset.x++; }
	ImGui::SetCursorPosX(335.f);
	if (ImGui::ArrowButton("##Down", ImGuiDir_Down)) { m_vecAnimRect[m_AnimIdx].vOffset.y++; }
	ImGui::SetCursorPos(ImVec2(265.f,300.f)); ImGui::Separator();

	ImGui::Text("Frame Length"); ImGui::SameLine();
	ImU8 frmlength = m_vecAnimRect.size(); // Frame num  실시간 변경 필요
	static bool length_step = true;
	ImGui::PushItemWidth(110);
	ImGui::InputScalar("##FrameLength", ImGuiDataType_U8, &frmlength, length_step ? &frmUnit : NULL, NULL, "%u");
	if (frmlength > m_vecAnimRect.size() - 1) frmlength = m_vecAnimRect.size() - 1;

	ImGui::SameLine();
	ImGui::Text("FPS"); ImGui::SameLine();
	static bool fps_step = true;
	const float fpsUnit = 1.f;
	ImGui::InputScalar("##FPS", ImGuiDataType_Float, &m_fps, fps_step ? &fpsUnit : NULL, NULL, "%0.f");
	if (m_fps <= 0) m_fps = 0.01;
	ImGui::PopItemWidth(); 

	ImGui::SameLine();
	ImGui::Text("Loop");
	static bool bLoop = true;
	ImGui::SameLine();
	ImGui::Checkbox("##Loop", &bLoop);

	static bool bPlay = false;
	static bool bPause = false;
	static bool bfinish = false;
	static float ftime = 0.f;
	if (ImGui::Button("Play")) 
	{
		bPlay = true;
		bfinish = false;
		if (bPause)
			bPause = false;
		else
		{ 
			m_AnimIdx = 0; 
			ftime = 0.f;
		}
	}
	ImGui::SameLine();
	if (bPlay && ImGui::Button("Pause"))
	{
		bPause = true;
		bPlay = false;
	}
	ImGui::SameLine();
	ImGui::Text("Current Frame : %d", m_AnimIdx);
	
	if (bPlay)
	{
		ftime += DT_ENGINE;
		if (1.f / m_fps < ftime)
		{
			++m_AnimIdx;
			if (m_vecAnimRect.size() <= m_AnimIdx) // frmlength 연결필요
			{
				m_AnimIdx = (int)m_vecAnimRect.size() - 1;
				bfinish = true;
			}
			ftime = 0.f;
		}

		if (bfinish && bLoop)
		{
			m_AnimIdx = 0;
			bfinish = false;
		}
	}

	// Anim 나열
	//ImGui::Dummy(ImVec2(0.f, 500.f));
	ImGui::BeginChild("child", ImVec2(ImGui::GetContentRegionAvail().x, 125.f), ImGuiChildFlags_Border, ImGuiWindowFlags_HorizontalScrollbar| ImGuiWindowFlags_NoScrollWithMouse);
	if (nullptr != m_CurAtlas)
	{
		for (int i = 0; i < m_vecAnimRect.size(); i++)
		{
			ImGuiWindow* window = ImGui::GetCurrentWindow();
			ImGuiContext& g = *GImGui;
			const ImVec2 padding = g.Style.FramePadding;

			ImVec4 col = ImVec4(1, 1, 1, 1);
			ImVec2 displayLT = ImVec2(m_vecAnimRect[i].vLeftTop);
			ImVec2 displayRB = ImVec2(m_vecAnimRect[i].vLeftTop + m_vecAnimRect[i].vSlice);
			ImVec2 displaySize = ImVec2(m_vecAnimRect[i].vSlice);
			UINT texturewidth = (m_CurAtlas.Get()->GetWidth());
			UINT textureheight = (m_CurAtlas.Get()->GetHeight());

			ImVec2 uv0 = ImVec2(displayLT.x / texturewidth, displayLT.y / textureheight);
			ImVec2 uv1 = ImVec2((displayLT.x + displaySize.x) / texturewidth, (displayLT.y + displaySize.y) / textureheight);

			// rect 선택
			// ImVec2(100.f, 100.f) == 애니메이션 사이즈
			const ImRect select(window->DC.CursorPos, window->DC.CursorPos + ImVec2(100.f, 100.f) + padding * 2.0f);
			if (select.Contains(io.MousePos) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				m_AnimIdx = i;

			if (i == m_AnimIdx) // 선택된 rect 테두리 색상
				col = ImVec4(1, 0, 0, 1);

			ImGui::Image(m_CurAtlas.Get()->GetSRV().Get(), ImVec2(100.f, 100.f), uv0, uv1, ImVec4(1, 1, 1, 1), col);
			ImGui::SameLine();
		}
	}
	ImGui::SetScrollX(ImGui::GetScrollX() - (15.f * io.MouseWheel));
	ImGui::EndChild();
	ImGui::End();
}

void AnimationEditorUI::DrawCanvas()
{
	// canvas
	static ImVector<ImVec2> points;
	static ImVec2 scrolling(0.0f, 0.0f);
	static float WheelSz = 1.f;
	static bool opt_enable_grid = true;
	static bool cutting_rect = false;
	static bool opt_enable_context_menu = true;

	ImGui::Checkbox("Enable grid", &opt_enable_grid);
	ImGui::SameLine();
	ImGui::Checkbox("Enable context menu", &opt_enable_context_menu);
	ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      
	ImVec2 canvas_sz = ImGui::GetContentRegionAvail() ;  
	if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
	if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
	ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

	m_CanvasLeftTop = canvas_p0;
	m_CenterPos = canvas_sz / 2;
	// Draw border and background color
	ImGuiIO& io = ImGui::GetIO();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));

	// This will catch our interactions
	ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
	const bool is_hovered = ImGui::IsItemHovered(); // Hovered
	const bool is_active = ImGui::IsItemActive();   // Held

	m_CenterPos -= scrolling;
	ImVec2 WheelOffset = m_CenterPos * WheelSz - m_CenterPos;
	const ImVec2 origin(canvas_p0.x + scrolling.x - WheelOffset.x, canvas_p0.y + scrolling.y - WheelOffset.y);
	const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);

	m_MousePos = mouse_pos_in_canvas;
	
	// Canvas 안에 이미지출력
	if (nullptr != m_CurAtlas)
	{
		draw_list->PushClipRect(canvas_p0, canvas_p1, true);
		ComPtr<ID3D11ShaderResourceView> my_texture = NULL;

		my_texture = m_CurAtlas.Get()->GetSRV().Get();
		float my_image_width = m_CurAtlas.Get()->GetWidth() * WheelSz;
		float my_image_height = m_CurAtlas.Get()->GetHeight() * WheelSz;

		ImVec2 left_top = m_CanvasLeftTop + ImVec2(scrolling.x, scrolling.y) - WheelOffset;
		ImVec2 right_bottom = (left_top + ImVec2(my_image_width, my_image_height));

		draw_list->AddImage((void*)my_texture.Get(), left_top, right_bottom);
		draw_list->AddRect(left_top, right_bottom, IM_COL32(255, 255, 255, 255)); // 아틀라스테두리
		draw_list->PopClipRect();
	}

	// 좌클릭시
	if (ImGui::IsWindowFocused() && m_bSlice && is_hovered && !cutting_rect && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	{
		points.push_back(mouse_pos_in_canvas / WheelSz);
		points.push_back(mouse_pos_in_canvas / WheelSz);

		cutting_rect = true;
	}
	if (cutting_rect)
	{
		points.back() = mouse_pos_in_canvas / WheelSz;

		if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
		{
			cutting_rect = false;
			ImRect rect(points[points.size() - 2],points.back());
			if (rect.Min.x > rect.Max.x) ImSwap(rect.Min.x, rect.Max.x);
			if (rect.Min.y > rect.Max.y) ImSwap(rect.Min.y, rect.Max.y);
			m_vecRect.push_back(rect);
			m_bSlice = false;
		}
	}

	// 오른쪽 마우스 드래그
	const float mouse_threshold_for_pan = opt_enable_context_menu ? -1.0f : 0.0f;
	if (ImGui::IsWindowFocused() && is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, mouse_threshold_for_pan))
	{
		scrolling.x += io.MouseDelta.x;
		scrolling.y += io.MouseDelta.y;
		m_Scrolling = scrolling;
	}

	// 마우스 휠
	if (ImGui::IsWindowFocused() && (io.MouseWheel > 0.f || io.MouseWheel))
	{
		ImRect Incanvas(canvas_p0, canvas_p1);
		if (Incanvas.Contains(io.MousePos))
		{
			WheelSz += 0.1f * io.MouseWheel;
			if (WheelSz <= 0.f) WheelSz = 0.f;
			m_Wheelsz = WheelSz;
		}
	}

	// Draw grid 
	draw_list->PushClipRect(canvas_p0, canvas_p1, true);
	if (opt_enable_grid)
	{
		const float GRID_STEP = 64.0f;
		for (float x = fmodf(scrolling.x , GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
			draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), IM_COL32(200, 200, 200, 40));
		for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
			draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), IM_COL32(200, 200, 200, 40));
	} 

	if (m_bSmartSlice)
	{
		SmartSlice(points);
	}

	// draw rect
	//static int selectidx = -1;	 // 나중에 여러 개 선택되게
	for (int n = 0; n < points.Size; n += 2)
	{
		ImGuiContext& g = *GImGui;
		const ImVec2 padding = g.Style.FramePadding;
		ImU32 col = IM_COL32(255, 255, 0, 255);

		ImVec2 leftTop = points[n] * WheelSz + origin;
		ImVec2 rightBottom  = points[n+1] * WheelSz + origin;

		if (leftTop.x > rightBottom.x) ImSwap(leftTop.x, rightBottom.x);
		if (leftTop.y > rightBottom.y) ImSwap(leftTop.y, rightBottom.y);


		const ImRect select(leftTop, rightBottom + padding * 2.0f);
		if (ImGui::IsWindowFocused() && select.Contains(io.MousePos) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			m_CanvasIdx = n / 2;

		// MouseGrip
		MouseGrip(io.MousePos, mouse_pos_in_canvas, leftTop, rightBottom, points, n, WheelSz);

		if (n / 2  == m_CanvasIdx)
		{
			col = IM_COL32(255, 0, 0, 255);

			if (m_bTrim)
			{
				m_bTrim = false;
				ImRect rec = TrimAtlas(m_CanvasIdx);

				points[n] = rec.Min;
				points[n + 1] = rec.Max;
				m_vecRect[n / 2] = rec;
			}
		}
		
		draw_list->AddRect(leftTop ,rightBottom , col, 2.0f);
	}

	draw_list->PopClipRect();
}

void AnimationEditorUI::SelectAtlas(DWORD_PTR _ptr)
{
	string strAnim = (char*)_ptr;
	wstring strAnimName = ToWString(strAnim);

	Ptr<CTexture> Tex = CAssetMgr::GetInst()->FindAsset<CTexture>(strAnimName);
	m_CurAtlas = Tex;
}

void AnimationEditorUI::MouseGrip(const ImVec2& _ioMousePos, const ImVec2& _canvasMousePos, 
	const ImVec2& _LT, const ImVec2& _RB, ImVector<ImVec2>& _points, const float _idx, const float _wheel )
{
	if (!ImGui::IsWindowFocused())
		return;

	// Mouse Grip Cursur
	// 왼쪽
	if (_LT.x - 10.f < _ioMousePos.x &&
		_LT.x + 10.f > _ioMousePos.x &&
		_LT.y <= _ioMousePos.y &&
		_RB.y > _ioMousePos.y)
	{
		ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
		static bool grip_resize = false;
		if (!grip_resize && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			grip_resize = true;

		if (grip_resize)
		{
			_points[_idx].x = _canvasMousePos.x / _wheel;
			if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
			{
				grip_resize = false;
				m_vecRect[_idx / 2].Min.x = _points[_idx].x;
			}
		}

	}

	// 오른쪽
	if (_RB.x - 10.f < _ioMousePos.x &&
		_RB.x + 10.f > _ioMousePos.x &&
		_LT.y <= _ioMousePos.y &&
		_RB.y > _ioMousePos.y)
	{
		ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
		static bool grip_resize = false;
		if (!grip_resize && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			grip_resize = true;

		if (grip_resize)
		{
			_points[_idx + 1].x = _canvasMousePos.x / _wheel;
			if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
			{
				grip_resize = false;
				m_vecRect[_idx / 2].Max.x = _points[_idx + 1].x;
			}
		}
	}

	// 위
	if (_LT.y - 10.f < _ioMousePos.y &&
		_LT.y + 10.f > _ioMousePos.y &&
		_LT.x <= _ioMousePos.x &&
		_RB.x > _ioMousePos.x)
	{
		ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
		static bool grip_resize = false;
		if (!grip_resize && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			grip_resize = true;

		if (grip_resize)
		{
			_points[_idx].y = _canvasMousePos.y / _wheel;
			if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
			{
				grip_resize = false;
				m_vecRect[_idx / 2].Min.y = _points[_idx].y;
			}
		}
	}

	// 아래
	if (_RB.y - 10.f < _ioMousePos.y &&
		_RB.y + 10.f > _ioMousePos.y &&
		_LT.x <= _ioMousePos.x &&
		_RB.x > _ioMousePos.x)
	{
		ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
		static bool grip_resize = false;
		if (!grip_resize && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			grip_resize = true;

		if (grip_resize)
		{
			_points[_idx + 1].y = _canvasMousePos.y / _wheel;
			if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
			{
				grip_resize = false;
				m_vecRect[_idx / 2].Max.y = _points[_idx + 1].y;
			}
		}
	}
}

ImRect AnimationEditorUI::TrimAtlas(int _idx)
{
	ImRect rec = m_vecRect[_idx];
	int rec_width = rec.GetWidth();
	int rec_height = rec.GetHeight();
	const int atlas_width = m_CurAtlas->GetWidth();
	const int atlas_height = m_CurAtlas->GetHeight();

	// slice rec가 atlas범위를 벗어날경우
	if (rec.Min.x < 0) { rec_width += rec.Min.x; rec.Min.x = 0; }
	if (rec.Min.y < 0) { rec_height += rec.Min.y; rec.Min.y = 0; }
	if (rec.Max.x > atlas_width) rec_width = atlas_width - rec.Min.x;
	if (rec.Max.y > atlas_height) rec_height = atlas_height - rec.Min.y;

	ImVec2 leftTop = ImVec2(atlas_width + 1.f, atlas_height + 1.f);
	ImVec2 rightBottom = ImVec2(-1.f, -1.f);

	tPixel* pPixel = m_CurAtlas.Get()->GetPixels(); 
	for (int i = 0; i < rec_height; i++)
	{
		for (int j = 0; j < rec_width; j++)
		{
			int x = rec.Min.x + j;
			int y = rec.Min.y + i;
			tPixel pixel = pPixel[(atlas_width * y) + x];
			if (0 != pixel.a)
			{
				if (x < leftTop.x) leftTop.x = x;
				if (y < leftTop.y) leftTop.y = y;
			}
			else if (0 != pPixel[(atlas_width * y) + x - 1].a ||
					(0 != y && 0 != pPixel[(atlas_width * (y - 1)) + x].a))
			{
				if (x > rightBottom.x) rightBottom.x = x;
				if (y > rightBottom.y) rightBottom.y = y;
			}
		}
	}

	rec.Min = leftTop;
	rec.Max = rightBottom;

	return rec;
}

void AnimationEditorUI::SmartSlice(ImVector<ImVec2>& _points)
{
	_points.clear();	// 기존 points vector를 비워줌
	m_bSmartSlice = false;

	tPixel* pPixel = m_CurAtlas.Get()->GetPixels();
	const int width = m_CurAtlas->GetWidth();
	const int height = m_CurAtlas->GetHeight();
	vector<vector<bool>> visit(height,vector<bool>(width,false)); // 방문 pixel

	const int dir[4][2] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} }; // 상 하 좌 우

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (visit[y][x])
				continue;

			ImVec2 leftTop = ImVec2(m_CurAtlas->GetWidth() + 1.f, m_CurAtlas->GetHeight() + 1.f);
			ImVec2 rightBottom = ImVec2(-1.f, -1.f);

			tPixel pixel = pPixel[width * y + x];
			if (0 != pixel.a)
			{
				queue<ImVec2> qPixel;
				qPixel.push(ImVec2(x, y));
				while (!qPixel.empty())
				{
					ImVec2 dot = qPixel.front();
					visit[dot.y][dot.x] = true;

					for (int i = 0; i < 4; i++)
					{
						int newX = dot.x + dir[i][0];
						int newY = dot.y + dir[i][1];

						if (dot.x < leftTop.x) leftTop.x = dot.x;
						if (dot.y < leftTop.y) leftTop.y = dot.y;
						if (newX > rightBottom.x) rightBottom.x = newX;
						if (newY > rightBottom.y) rightBottom.y = newY;

						if (newX < 0 || newX >= width || newY < 0 || newY >= height)
							continue;

						tPixel pixel = pPixel[width * newY + newX];
						if (0 != pixel.a && !visit[newY][newX])
						{
							qPixel.push(ImVec2(newX, newY));
						}

						visit[newY][newX] = true;
					}
					qPixel.pop();
				}
				// LT,RB points에 저장
				_points.push_back(leftTop);
				_points.push_back(rightBottom);
				m_vecRect.push_back(ImRect(leftTop, rightBottom));
			}
		}
	}

}

void AnimationEditorUI::Deactivate()
{
	UI::Deactivate();
}

	// 오른쪽 마우스 메뉴
//ImVec2 drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
//if (opt_enable_context_menu && drag_delta.x == 0.0f && drag_delta.y == 0.0f)
//ImGui::OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
//if (ImGui::BeginPopup("context"))
//{
//	if (cutting_rect)
//		points.resize(points.size() - 2);
//	cutting_rect = false;
//	if (ImGui::MenuItem("Remove one", NULL, false, points.Size > 0))
//	{
//		points.resize(points.size() - 2);
//	}
//	if (ImGui::MenuItem("Remove all", NULL, false, points.Size > 0))
//	{
//
//		points.clear();
//	}
//	ImGui::EndPopup();
//}

// 줄에 걸치면 ImGui::IsItemHovered() ImGui::SetMouseCursor(ImGuiMouseCursor_COUNT)

