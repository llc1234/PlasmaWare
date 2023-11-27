#pragma once

class AutoStrafe {
	bool one_stop_space = false;
	float rot[3] = {0, 0, 0};

public:
	void loop(HANDLE hProcess) {
		if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
			uintptr_t localplayer;
			ReadProcessMemory(hProcess, (void*)(client_DLL + dwlocalplayer), &localplayer, sizeof(localplayer), 0);

			float new_rot[3] = {0, 0, 0};
			ReadProcessMemory(hProcess, (void*)(localplayer + m_viewPunchAngle), &new_rot, sizeof(new_rot), 0);

			// int v = 0;
			// WriteProcessMemory(hProcess, (void*)(client_DLL + dwForceLeft), &v, sizeof(v), 0);
			// WriteProcessMemory(hProcess, (void*)(client_DLL + dwForceRight), &v, sizeof(v), 0);

			if (new_rot[1] > rot[1]) {
				int v = 1;
				WriteProcessMemory(hProcess, (void*)(client_DLL + dwForceLeft), &v, sizeof(v), 0);
				v = 0;
				WriteProcessMemory(hProcess, (void*)(client_DLL + dwForceRight), &v, sizeof(v), 0);

				rot[1] = new_rot[1];
				one_stop_space = true;

			}
			else if (new_rot[1] < rot[1]) {
				int v = 1;
				WriteProcessMemory(hProcess, (void*)(client_DLL + dwForceRight), &v, sizeof(v), 0);
				v = 0;
				WriteProcessMemory(hProcess, (void*)(client_DLL + dwForceLeft), &v, sizeof(v), 0);

				rot[1] = new_rot[1];
				one_stop_space = true;

			}

		}
		else {
			if (one_stop_space) {
				int v = 0;
				WriteProcessMemory(hProcess, (void*)(client_DLL + dwForceRight), &v, sizeof(v), 0);
				v = 0;
				WriteProcessMemory(hProcess, (void*)(client_DLL + dwForceLeft), &v, sizeof(v), 0);

				one_stop_space = false;
			}
		}
	}
};