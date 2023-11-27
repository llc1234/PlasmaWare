#pragma once
class Menu {
public:
	bool c_TriggerBot = false;

	bool c_Bhop = false;

	bool c_autostrafe = false;

	bool c_Radar = false;

	bool c_OutOfFovArrows = false;

	bool c_AimBot = false;
	bool c_AimBot_autoshoot = false;
	bool c_AimBot_circle = false;
	float v_AimBot_distance = 1000;
	float v_AimBot_Fov = 3;

	bool c_Esp = false;
	bool v_Esp_box = false;
	bool v_Esp_line = false;
	float v_Esp_distance = 1000;

	int menu_tab = 0;
	int menu_pos_y = 1;
	int menu_line_pos = 0;
	int menu_line2_pos = 423;

	void setup() {
		imgui_syle();
	}

	void tab1() {
		ImGui::SetCursorPos(ImVec2(100, 15));
		ImGui::BeginChild("c1", ImVec2(230, 470), true, 1);

		ImGui::Checkbox("AimBot", &c_AimBot);
		ImGui::Checkbox("Autoshoot", &c_AimBot_autoshoot);
		ImGui::SliderFloat("Distance", &v_AimBot_distance, 0, 2500);
		ImGui::SliderFloat("Fov", &v_AimBot_Fov, 1, 24);

		ImGui::EndChild();


		ImGui::SetCursorPos(ImVec2(350, 15));
		ImGui::BeginChild("c2", ImVec2(230, 470), true, 1);

		ImGui::Checkbox("TriggerBot", &c_TriggerBot);

		ImGui::EndChild();
	}

	void tab2() {
		ImGui::SetCursorPos(ImVec2(100, 15));
		ImGui::BeginChild("c1", ImVec2(230, 470), true, 1);

		ImGui::Checkbox("Bhop", &c_Bhop);
		ImGui::Checkbox("AutoStrafe", &c_autostrafe);
		
		ImGui::EndChild();


		ImGui::SetCursorPos(ImVec2(350, 15));
		ImGui::BeginChild("c2", ImVec2(230, 470), true, 1);

		
		ImGui::EndChild();
	}

	void tab3() {
		ImGui::SetCursorPos(ImVec2(100, 15));
		ImGui::BeginChild("c1", ImVec2(230, 470), true, 1);

		ImGui::Checkbox("Esp", &c_Esp);
		ImGui::Checkbox("Esp box", &v_Esp_box);
		ImGui::Checkbox("Esp Line", &v_Esp_line);
		ImGui::SliderFloat("Esp distance", &v_Esp_distance, 0, 2500);
		
		ImGui::Checkbox("AimBot Circle", &c_AimBot_circle);

		ImGui::EndChild();


		ImGui::SetCursorPos(ImVec2(350, 15));
		ImGui::BeginChild("c2", ImVec2(230, 470), true, 1);

		ImGui::Checkbox("Radar", &c_Radar);
		ImGui::Checkbox("OutOfFovArrows", &c_OutOfFovArrows);

		ImGui::EndChild();
	}

	void draw_side_line() {
		ImVec2 windowPos = ImGui::GetWindowPos();

		ImVec2 lineStart = ImVec2(windowPos.x + 77, windowPos.y);
		ImVec2 lineEnd = ImVec2(windowPos.x + 77, windowPos.y + menu_line_pos);
		ImGui::GetWindowDrawList()->AddLine(lineStart, lineEnd, IM_COL32(90, 90, 90, 255), 1.0f);

		ImVec2 lineStart2 = ImVec2(windowPos.x + 77, windowPos.y + menu_line_pos);
		ImVec2 lineEnd2 = ImVec2(windowPos.x, windowPos.y + menu_line_pos);
		ImGui::GetWindowDrawList()->AddLine(lineStart2, lineEnd2, IM_COL32(90, 90, 90, 255), 1.0f);

		ImVec2 lineStart3 = ImVec2(windowPos.x + 77, windowPos.y + 500);
		ImVec2 lineEnd3 = ImVec2(windowPos.x + 77, windowPos.y + 500 - menu_line2_pos);
		ImGui::GetWindowDrawList()->AddLine(lineStart3, lineEnd3, IM_COL32(90, 90, 90, 255), 1.0f);

		ImVec2 lineStart4 = ImVec2(windowPos.x + 77, windowPos.y + 500 - menu_line2_pos);
		ImVec2 lineEnd4 = ImVec2(windowPos.x, windowPos.y + 500 - menu_line2_pos);
		ImGui::GetWindowDrawList()->AddLine(lineStart4, lineEnd4, IM_COL32(90, 90, 90, 255), 1.0f);
	}

	void menu_side_buttons() {
		ImVec4* colors = ImGui::GetStyle().Colors;

		// button 1
		colors[ImGuiCol_Button] = ImVec4(0.03f, 0.03f, 0.03f, 1.00f);
		if (menu_tab == 0) {
			colors[ImGuiCol_Button] = ImVec4(0.07f, 0.07f, 0.07f, 1.00f);
		}

		ImGui::SetCursorPos(ImVec2(5, 5));
		if (ImGui::Button("Legit", ImVec2(71.4, 71.4))) {
			menu_tab = 0;
			menu_line_pos = 0;
			menu_line2_pos = 423;
		}

		// button 2

		colors[ImGuiCol_Button] = ImVec4(0.03f, 0.03f, 0.03f, 1.00f);
		if (menu_tab == 1) {
			colors[ImGuiCol_Button] = ImVec4(0.07f, 0.07f, 0.07f, 1.00f);
		}

		ImGui::SetCursorPos(ImVec2(5, 76));
		if (ImGui::Button("Misc", ImVec2(71.4, 71.4))) {
			menu_tab = 1;
			menu_line_pos = 77;
			menu_line2_pos = 355;
		}

		// button 3

		colors[ImGuiCol_Button] = ImVec4(0.03f, 0.03f, 0.03f, 1.00f);
		if (menu_tab == 2) {
			colors[ImGuiCol_Button] = ImVec4(0.07f, 0.07f, 0.07f, 1.00f);
		}

		ImGui::SetCursorPos(ImVec2(5, 73.5 * 2));
		if (ImGui::Button("Visuals", ImVec2(71.4, 71.4))) {
			menu_tab = 2;
			menu_line_pos = 73 * 2;
			menu_line2_pos = 283;
		}

		draw_side_line();
	}

	void main_menu() {
		ImGui::SetNextWindowSize(ImVec2(600, 500));
		ImGui::Begin("PlasmaWare V0.1.0", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

		menu_side_buttons();

		if (menu_tab == 0) {
			tab1();
		} else if (menu_tab == 1) {
			tab2();
		} else if(menu_tab == 2) {
			tab3();
		}

		ImGui::End();
	}

	void loop() {
		main_menu();
	}

	void imgui_syle() {
		ImVec4* colors = ImGui::GetStyle().Colors;
		// colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		// colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.07f, 0.07f, 0.07f, 1.00f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
		// colors[ImGuiCol_Border] = ImVec4(0.9f, 0.9f, 0.9f, 1.00f);
		// colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
		// colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
		// colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
		// colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
		// colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		// colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
		// colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		// colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		// colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
		// colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
		// colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
		// colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
		colors[ImGuiCol_CheckMark] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
		// colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
		// colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
		colors[ImGuiCol_Button] = ImVec4(0.03f, 0.03f, 0.03f, 1.00f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.07f, 0.07f, 0.07f, 1.00f);
		// colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
		// colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
		// colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
		// colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
		// colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
		// colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
		// colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
		// colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
		// colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
		// colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
		// colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
		// colors[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		// colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
		// colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
		// colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		// colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		// colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		// colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		// colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		// colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
		// colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
		// colors[ImGuiCol_TableBorderLight] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
		// colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		// colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
		// colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
		// colors[ImGuiCol_DragDropTarget] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
		// colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		// colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
		// colors[ImGuiCol_NavWindowingDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
		// colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);


		ImGuiStyle& style = ImGui::GetStyle();


		// style.WindowPadding = ImVec2(8.00f, 8.00f);
		// style.FramePadding = ImVec2(5.00f, 2.00f);
		// style.CellPadding = ImVec2(6.00f, 6.00f);
		// style.ItemSpacing = ImVec2(6.00f, 6.00f);
		// style.ItemInnerSpacing = ImVec2(6.00f, 6.00f);
		// style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
		// style.IndentSpacing = 25;
		// style.ScrollbarSize = 15;
		// style.GrabMinSize = 10;
		// style.WindowBorderSize = 1;
		// style.ChildBorderSize = 1;
		// style.PopupBorderSize = 1;
		// style.FrameBorderSize = 1;
		// style.TabBorderSize = 1;
		// style.WindowRounding = 7;
		// style.ChildRounding = 4;
		// style.FrameRounding = 3;
		// style.PopupRounding = 4;
		// style.ScrollbarRounding = 9;
		// style.GrabRounding = 3;
		// style.LogSliderDeadzone = 4;
		// style.TabRounding = 4;
	}
};