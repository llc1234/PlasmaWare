#pragma once

uintptr_t client_DLL = 0;
uintptr_t engine_DLL = 0;

uintptr_t dwlocalplayer = 0xCC52F8;
uintptr_t dwEntitylist = 0xC57504;
uintptr_t dwClientState = 0x3AE2CC;
uintptr_t dwViewMatrix = 0x9C;


uintptr_t dwForceAttack = 0xC9084C;
uintptr_t dwForceJump = 0xC90840;
uintptr_t dwForceLeft = 0xC8F83C;
uintptr_t dwForceRight = 0xC8F848;
uintptr_t dwViewRot = 0x46A5DC;

uintptr_t m_iflags = 0x37C;
uintptr_t m_iHealth = 0xA8;
uintptr_t m_vecOrigin = 0x28C;
uintptr_t m_vecVelocity = 0x120;
uintptr_t m_viewPunchAngle = 0x298;
uintptr_t m_iCrosshairId = 0x1884;

uintptr_t iTeamNum = 0xB0;
uintptr_t bDormant = 0x1AA;

uintptr_t EntityList_viewRot = 0x1E00;

// const char* p_dwlocalplayer = "60 B6 ? ? 00 00 00 00 01 00 00 00 7C E6 ? ? C8 1D ? ? 08";
// const char* p_dwEntitylist = "04 75 ? ? 04 75 ? ? 00 00 00 00 ?";
// const char* p_dwViewRot = "C1 ? C5 5B";

class Pattern {
public:
    MODULEENTRY32 get_module(const char* modName, DWORD proc_id) {
        HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, proc_id);
        if (hSnap != INVALID_HANDLE_VALUE) {
            MODULEENTRY32 modEntry;
            modEntry.dwSize = sizeof(modEntry);
            if (Module32First(hSnap, &modEntry)) {
                do {
                    if (!strcmp(modEntry.szModule, modName)) {
                        CloseHandle(hSnap);
                        return modEntry;
                    }
                } while (Module32Next(hSnap, &modEntry));
            }
        }
        MODULEENTRY32 module = { -1 };
        return module;
    }

    uintptr_t find_pattern(HANDLE process_handle, MODULEENTRY32 module, uint8_t* arr, const char* pattern, int offset, int extra) {
        uintptr_t scan = 0x0;
        const char* pat = pattern;
        uintptr_t firstMatch = 0;

        for (uintptr_t pCur = (uintptr_t)arr; pCur < (uintptr_t)arr + module.modBaseSize; ++pCur) {
            if (!*pat) {
                scan = firstMatch; break;
            }

            if (*(uint8_t*)pat == '\?' || *(uint8_t*)pCur == ((((pat[0] & (~0x20)) >= 'A' && (pat[0] & (~0x20)) <= 'F') ? ((pat[0] & (~0x20)) - 'A' + 0xa) : ((pat[0] >= '0' && pat[0] <= '9') ? pat[0] - '0' : 0)) << 4 | (((pat[1] & (~0x20)) >= 'A' && (pat[1] & (~0x20)) <= 'F') ? ((pat[1] & (~0x20)) - 'A' + 0xa) : ((pat[1] >= '0' && pat[1] <= '9') ? pat[1] - '0' : 0)))) {
                if (!firstMatch) {
                    firstMatch = pCur;
                }

                if (!pat[2]) {
                    scan = firstMatch; break;
                }

                if (*(WORD*)pat == 16191 /*??*/ || *(uint8_t*)pat != '\?') {
                    pat += 3;
                }
                else {
                    pat += 2;
                }
            }

            else {
                pat = pattern; firstMatch = 0;
            }
        }
        if (!scan) return 0x0;
        uint32_t read;
        ReadProcessMemory(process_handle, (void*)(scan - (uintptr_t)arr + (uintptr_t)module.modBaseAddr + offset), &read, sizeof(read), NULL);
        return read + extra;
    }

	void scan(HANDLE hProcess) {
        HWND hwnd = FindWindowA(NULL, "Team Fortress 2");
        DWORD proc_id;
        GetWindowThreadProcessId(hwnd, &proc_id);
        HANDLE process_handle = OpenProcess(PROCESS_VM_READ | PROCESS_VM_OPERATION | PROCESS_VM_WRITE, false, proc_id);

        MODULEENTRY32 client = get_module("client.dll", proc_id);
        MODULEENTRY32 engine = get_module("client.dll", proc_id);

        auto client_bytes = new uint8_t[client.modBaseSize];
        auto engine_bytes = new uint8_t[engine.modBaseSize];

        DWORD bytes_read;
        ReadProcessMemory(process_handle, client.modBaseAddr, client_bytes, client.modBaseSize, &bytes_read);
        ReadProcessMemory(process_handle, engine.modBaseAddr, engine_bytes, engine.modBaseSize, &bytes_read);
        

        // dwlocalplayer = find_pattern(process_handle, client, client_bytes, p_dwlocalplayer, 0x0, 0x0) + 0x199C98 - client_DLL;
        // dwEntitylist = find_pattern(process_handle, client, client_bytes, p_dwEntitylist, 0x0, 0x0) + 0x0 - client_DLL;
        // dwViewRot = find_pattern(process_handle, engine, engine_bytes, p_dwViewRot, 0x0, 0x0) - 0x79615A24 + engine_DLL;

        delete[] client_bytes;
        delete[] engine_bytes;

        // printf("[+] Found dwlocalplayer 0xCC52F8 new @ 0x%X\n", dwlocalplayer);
        // printf("[+] Found dwEntitylist  0xC57504 new @ 0x%X\n", dwEntitylist);
        // printf("[+] Found dwViewRot     0x46A5DC new @ 0x%X\n", dwViewRot);
	}
};