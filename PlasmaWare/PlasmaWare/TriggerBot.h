#pragma once

class TriggerBot {
	bool one = false;
	
public:
	void loop(HANDLE hProcess) {

		uintptr_t localplayer;
		ReadProcessMemory(hProcess, (void*)(client_DLL + dwlocalplayer), &localplayer, sizeof(localplayer), 0);

		int value;
		ReadProcessMemory(hProcess, (void*)(localplayer + m_iCrosshairId), &value, sizeof(value), 0);

		if (GetAsyncKeyState(VK_SHIFT)) {
			if (value > 0 && value < 25) {
				int attack = 5;
				WriteProcessMemory(hProcess, (void*)(client_DLL + dwForceAttack), &attack, sizeof(attack), 0);

			}
			else {
				int attack = 4;
				WriteProcessMemory(hProcess, (void*)(client_DLL + dwForceAttack), &attack, sizeof(attack), 0);

			}

			one = true;
		} else {
			if (one) {
				one = false;
				int v;
				ReadProcessMemory(hProcess, (void*)(client_DLL + dwForceAttack), &v, sizeof(v), 0);

				if (v > 4) {
					int attack = 4;
					WriteProcessMemory(hProcess, (void*)(client_DLL + dwForceAttack), &attack, sizeof(attack), 0);

				}
			}
		}
	}
};