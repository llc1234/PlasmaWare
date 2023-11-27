#pragma once

class AimBot {
public:
	bool autoshoot = false;
	bool circle = false;
	float distance = 0;
	float Fov = 50;

	float map(float x, float in_min, float in_max, float out_min, float out_max) {
		return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	}

	void draw_Fov_circle() {
		float dis = map(Fov, 0, 24, 0, 0.5);

		if (circle) {
			glBegin(GL_LINES);

			glColor3f(1, 1, 1);
			// glVertex2f(-0.5, 0);
			// glVertex2f(0.5, 0);

			float r = 0;

			for (int i = 0; i < 70; i++) {
				glVertex2f(0 + sin(r) * dis, 0 + cos(r) * (dis * 1.6));
				glVertex2f(0 + sin(r + 0.1) * dis, 0 + cos(r + 0.1) * (dis * 1.6));

				r += 0.1;
			}

			glEnd();
		}
	}

	void loop(HANDLE hProcess) {
		draw_Fov_circle();

		uintptr_t local;
		ReadProcessMemory(hProcess, (void*)(client_DLL + dwlocalplayer), &local, sizeof(local), 0);

		float local_pos[3];
		ReadProcessMemory(hProcess, (void*)(local + m_vecOrigin), &local_pos, sizeof(local_pos), 0);

		float local_rot[3];
		ReadProcessMemory(hProcess, (void*)(local + m_viewPunchAngle), &local_rot, sizeof(local_rot), 0);
		
		int local_team;
		ReadProcessMemory(hProcess, (void*)(local + iTeamNum), &local_team, sizeof(local_team), 0);

		float entitys_dis[64];

		for (int i = 0; i < 64; i++) {
			entitys_dis[i] = -1;
		}

		for (int i = 0; i < 64; i++) {
			uintptr_t entity;
			ReadProcessMemory(hProcess, (void*)(client_DLL + dwEntitylist + i * 0x10), &entity, sizeof(entity), 0);

			if (!entity) {
				continue;
			}

			int entity_Health;
			ReadProcessMemory(hProcess, (void*)(entity + m_iHealth), &entity_Health, sizeof(entity_Health), 0);

			if (!(entity_Health < 500 && entity_Health > 1)) {
				continue;
			}

			int entity_team;
			ReadProcessMemory(hProcess, (void*)(entity + iTeamNum), &entity_team, sizeof(entity_team), 0);

			if (entity_team == local_team) {
				continue;
			}

			float entity_pos[3];
			ReadProcessMemory(hProcess, (void*)(entity + m_vecOrigin), &entity_pos, sizeof(entity_pos), 0);


			float rot_x = atan2(entity_pos[0] - local_pos[0], entity_pos[1] - local_pos[1]);
			float rot_x_new = 0;

			float rot_y = atan2(-(local_pos[2] - (entity_pos[2] - 20)), hypot(entity_pos[0] - local_pos[0], entity_pos[1] - local_pos[1]));
			float rot_y_new = map(rot_y, -1.57075, 1.57075, 89, -89);

			if (rot_x < -1.58 && rot_x > -3.1415) {
				rot_x_new = map(rot_x + 4.70, -3.1415, 3.1415, 180, -180);
			} else {
				rot_x_new = map(rot_x - 1.5707, -3.1415, 3.1415, 180, -180); // + 6.28318;
			}

			// std::cout << "player: " << local_rot[0] << ", entity: " << rot_y_new << std::endl;
			
			if (local_rot[1] > rot_x_new - Fov && local_rot[1] < rot_x_new + Fov && local_rot[0] > rot_y_new - Fov && local_rot[0] < rot_y_new + Fov) {
				entitys_dis[i] = hypot(entity_pos[0] - local_pos[0], entity_pos[1] - local_pos[1]);
			}
		}



		int row = -1;
		int value = 50000;

		for (int i = 0; i < 64; i++) {
			if (entitys_dis[i] == -1) {

			} else if (value > entitys_dis[i]) {
				value = entitys_dis[i];
				row = i;
			}
		}

		if (row != -1 && entitys_dis[row] < distance) {
			uintptr_t entity;
			ReadProcessMemory(hProcess, (void*)(client_DLL + dwEntitylist + row * 0x10), &entity, sizeof(entity), 0);
			
			if (GetAsyncKeyState(VK_LBUTTON)) {
				if (entity) {
					int entity_Health;
					ReadProcessMemory(hProcess, (void*)(entity + m_iHealth), &entity_Health, sizeof(entity_Health), 0);

					if ((entity_Health < 500 && entity_Health > 1)) {
						float entity_pos[3];
						ReadProcessMemory(hProcess, (void*)(entity + m_vecOrigin), &entity_pos, sizeof(entity_pos), 0);


						float rot_x = atan2(entity_pos[0] - local_pos[0], entity_pos[1] - local_pos[1]);
						float rot_y = atan2(-(local_pos[2] - (entity_pos[2] - 20)), hypot(entity_pos[0] - local_pos[0], entity_pos[1] - local_pos[1]));

						float rot_x_new = map(rot_x + 1.57075, 0, 6.28, 180, -180);
						float rot_y_new = map(rot_y, -1.57075, 1.57075, 89, -89);

						WriteProcessMemory(hProcess, (void*)(engine_DLL + dwViewRot), &rot_y_new, sizeof(rot_y_new), 0);
						WriteProcessMemory(hProcess, (void*)(engine_DLL + dwViewRot + 0x4), &rot_x_new, sizeof(rot_x_new), 0);

						int value;
						ReadProcessMemory(hProcess, (void*)(local + m_iCrosshairId), &value, sizeof(value), 0);

						if (value > 0 && value < 25) {
							int attack = 5;
							WriteProcessMemory(hProcess, (void*)(client_DLL + dwForceAttack), &attack, sizeof(attack), 0);

						}
						else {
							int attack = 4;
							WriteProcessMemory(hProcess, (void*)(client_DLL + dwForceAttack), &attack, sizeof(attack), 0);

						}
					}
				}
			}
		}
	}
};