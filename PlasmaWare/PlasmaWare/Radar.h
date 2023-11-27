#pragma once

class Radar {
public:
	void drawrect(float x, float y) {
		glBegin(GL_QUADS);
		glVertex2f(x - 0.003, y - 0.005);
		glVertex2f(x + 0.003, y - 0.005);
		glVertex2f(x + 0.003, y + 0.005);
		glVertex2f(x - 0.003, y + 0.005);
		glEnd();
	}

	void drawrot(float x, float y, float rot, float dis) {
		glBegin(GL_LINES);
		glVertex2f(x, y);
		glVertex2f(x + sin(rot) * dis, y + cos(rot) * dis);
		glEnd();
	}

	float map(float x, float in_min, float in_max, float out_min, float out_max) {
		return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	}

	void loop(HANDLE hProcess) {
		glBegin(GL_QUADS);
		glColor3f(0, 0, 0);
		glVertex2f(-0.99, 0.99);
		glVertex2f(-0.60, 0.99);
		glVertex2f(-0.60, 0.45);
		glVertex2f(-0.99, 0.45);
		glEnd();

		

		uintptr_t local;
		ReadProcessMemory(hProcess, (void*)(client_DLL + dwlocalplayer), &local, sizeof(local), 0);

		float local_pos[3];
		ReadProcessMemory(hProcess, (void*)(local + m_vecOrigin), &local_pos, sizeof(local_pos), 0);

		float local_rot[3];
		ReadProcessMemory(hProcess, (void*)(local + m_viewPunchAngle), &local_rot, sizeof(local_rot), 0);

		int local_Team;
		ReadProcessMemory(hProcess, (void*)(local + iTeamNum), &local_Team, sizeof(local_Team), 0);



		for (int i = 0; i < 64; i++) {
			uintptr_t entity;
			ReadProcessMemory(hProcess, (void*)(client_DLL + dwEntitylist + i * 0x10), &entity, sizeof(entity), 0);

			int entity_health;
			ReadProcessMemory(hProcess, (void*)(entity + m_iHealth), &entity_health, sizeof(entity_health), 0);

			
			if (entity_health > 1 && entity_health < 500) {
				float entity_pos[3];
				ReadProcessMemory(hProcess, (void*)(entity + m_vecOrigin), &entity_pos, sizeof(entity_pos), 0);

				float entity_rot[2];
				ReadProcessMemory(hProcess, (void*)(entity + EntityList_viewRot), &entity_rot, sizeof(entity_rot), 0);


				int entity_Team;
				ReadProcessMemory(hProcess, (void*)(entity + iTeamNum), &entity_Team, sizeof(entity_Team), 0);


				float x = -0.79 + (entity_pos[0] - local_pos[0]) / 11000;
				float y = 0.68 + (entity_pos[1] - local_pos[1]) / 11000;

				if (x > -0.99 && x < -0.60 && y > 0.45 && y < 0.99) {
					if (entity_Team != local_Team) {
						glColor3f(1, 0, 0);
						drawrect(x, y);
						
						float n_rn = map(entity_rot[1] + 90, 180, -180, 0, 6.28318);
						drawrot(x, y, n_rn, 0.02);

						glColor3f(0, 1, 0);
						float n_r = map(local_rot[1] + 90, 180, -180, 0, 6.28318);
						drawrot(-0.79, 0.68, n_r, 0.08);
					}

					


					// std::cout << "x: " << entity_pos[0] << ", y: " << entity_pos[1] << ", z: " << entity_pos[2] << std::endl;

				}
			}
		}
	}
};