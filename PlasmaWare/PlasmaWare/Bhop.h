#pragma once

class Bhop {
public:
	void loop(HANDLE hProcess) {
		if (GetAsyncKeyState(VK_SPACE)) {
			uintptr_t localplayer;
			ReadProcessMemory(hProcess, (void*)(client_DLL + dwlocalplayer), &localplayer, sizeof(localplayer), 0);

			int value;
			ReadProcessMemory(hProcess, (void*)(localplayer + m_iflags), &value, sizeof(value), 0);

			if (value == 257) {
				int attack = 5;
				WriteProcessMemory(hProcess, (void*)(client_DLL + dwForceJump), &attack, sizeof(attack), 0);

			} else {
				int attack = 4;
				WriteProcessMemory(hProcess, (void*)(client_DLL + dwForceJump), &attack, sizeof(attack), 0);

			}
		}
	}
};