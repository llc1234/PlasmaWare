#include <vector>
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "menu.h"
#include "memory.h"

#include "ESP.h"
#include "Bhop.h"
#include "Radar.h"
#include "AimBot.h"
#include "AutoStrafe.h"
#include "TriggerBot.h"
#include "OutOfFovArrows.h"


// sv_cheats 1; nb_player_move 0; tf_bot_fire_weapon_allowed 0; nb_blind 1

class plasmaWare {
	HWND hwnd;
	DWORD procId;
	HANDLE hProcess;

	GLFWwindow* window;

	GLFWmonitor* PrimaryMonitor;

	bool menu_open = true;
	bool insert_button = true;

	Menu* menu = new Menu();

	Pattern* pattern = new Pattern();

	ESP* esp = new ESP();
	Bhop* bhop = new Bhop();
	Radar* radar = new Radar();
	AimBot* aimbot = new AimBot();
	AutoStrafe* autostrafe = new AutoStrafe();
	TriggerBot* triggerbot = new TriggerBot();
	OutOfFovArrows* outoffovarrows = new OutOfFovArrows();

public:
	uintptr_t GetModuleBaseAddress(const char* modName) {
		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
		if (hSnap != INVALID_HANDLE_VALUE) {
			MODULEENTRY32 modEntry;
			modEntry.dwSize = sizeof(modEntry);
			if (Module32First(hSnap, &modEntry)) {
				do {
					if (!strcmp(modEntry.szModule, modName)) {
						CloseHandle(hSnap);
						return (uintptr_t)modEntry.modBaseAddr;
					}
				} while (Module32Next(hSnap, &modEntry));
			}
		}
	}

	void imgui_setup() {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330");
	}

	void setup_glfw() {
		glfwInit();

		PrimaryMonitor = glfwGetPrimaryMonitor();

		int window_width = glfwGetVideoMode(PrimaryMonitor)->width;
		int window_height = glfwGetVideoMode(PrimaryMonitor)->height;

		glfwWindowHint(GLFW_FLOATING, true);
		glfwWindowHint(GLFW_RESIZABLE, false);
		glfwWindowHint(GLFW_MAXIMIZED, true);
		glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);

		window = glfwCreateWindow(window_width, window_height, "PlasmaWare", NULL, NULL);
		// window = glfwCreateWindow(1280, 768, "PlasmaWare", NULL, NULL);

		glfwSetWindowAttrib(window, GLFW_DECORATED, false);
		glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, false);

		glfwMakeContextCurrent(window);

		//glfwSetWindowPos(window, 640, 273);
	}

	void Find_winddw_and_DLL() {
		hwnd = FindWindowA(NULL, "Team Fortress 2");
		GetWindowThreadProcessId(hwnd, &procId);

		hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

		// while (client_DLL == 0 || engine_DLL == 0) {
			// client_DLL = GetModuleBaseAddress("client.dll");
			// 	engine_DLL = GetModuleBaseAddress("engine.dll");
		// }

		
		// std::cout << "hwnd:       " << hwnd << std::endl;
		// std::cout << "procId:     " << procId << std::endl;
		// std::cout << "hProcess:   " << hProcess << std::endl;

		// std::cout << "client_DLL: " << client_DLL << std::endl;
	}

	void setup() {
		Find_winddw_and_DLL();
		setup_glfw();
		imgui_setup();

		menu->setup();
		pattern->scan(hProcess);
	}

	bool WindowShouldClose() {
		return glfwWindowShouldClose(window);
	}

	void imgui_newframe() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void imgui_render() {
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void open_chose_menu() {
		if (insert_button && (GetKeyState(VK_INSERT) & 0x8000)) {
			insert_button = false;
			menu_open = !menu_open;
			
		}

		if (!(GetKeyState(VK_INSERT) & 0x8000)) {
			insert_button = true;
			
		}

		if (menu_open) {
			glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, false);

		} else {
			glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, true);

		}
	}

	void glfw_loop() {
		glClear(GL_COLOR_BUFFER_BIT);

		imgui_newframe();

		open_chose_menu();
		if (menu_open) {
			menu->loop();
		}
		imgui_render();
		
	}

	void glfw_end_loop() {
		glfwSwapBuffers(window);
		glfwPollEvents();
		glfwSwapInterval(0);
	}

	void loop() {
		glfw_loop();

		if (menu->c_TriggerBot) {
			triggerbot->loop(hProcess);
		}

		if (menu->c_Bhop) {
			bhop->loop(hProcess);
		}

		if (menu->c_autostrafe) {
			autostrafe->loop(hProcess);
		}

		if (menu->c_Radar) {
			radar->loop(hProcess);
		}

		if (menu->c_Esp) {
			esp->Esp_box = menu->v_Esp_box;
			esp->Esp_line = menu->v_Esp_line;
			esp->Esp_distance = menu->v_Esp_distance;

			esp->loop(hProcess);
		}

		if (menu->c_OutOfFovArrows) {
			outoffovarrows->loop(hProcess);
		}

		if (menu->c_AimBot) {
			aimbot->distance = menu->v_AimBot_distance;
			aimbot->autoshoot = menu->c_AimBot_autoshoot;

			aimbot->Fov = menu->v_AimBot_Fov;
			aimbot->circle = menu->c_AimBot_circle;

			aimbot->loop(hProcess);
		}

		glfw_end_loop();
	}
};

int main() {
	plasmaWare* plasma = new plasmaWare();
	
	plasma->setup();

	while (!plasma->WindowShouldClose()) {
		plasma->loop();
	}

	return 0;
}