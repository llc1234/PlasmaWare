#pragma once

class OutOfFovArrows {
	float map(float x, float in_min, float in_max, float out_min, float out_max) {
		return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	}

public:
	void loop(HANDLE handle) {
		uintptr_t local;
		ReadProcessMemory(handle, (void*)(client_DLL + dwlocalplayer), &local, sizeof(local), 0);

		int local_Team;
		ReadProcessMemory(handle, (void*)(local + iTeamNum), &local_Team, sizeof(local_Team), 0);

		float local_pos[3];
		ReadProcessMemory(handle, (void*)(local + m_vecOrigin), &local_pos, sizeof(local_pos), 0);



		float local_rot[2];
		ReadProcessMemory(handle, (void*)(local + m_viewPunchAngle), &local_rot, sizeof(local_rot), 0);



		glBegin(GL_TRIANGLES);
		glColor3f(1, 0, 0);

		for (int i = 0; i < 64; i++) {
			uintptr_t entity;
			ReadProcessMemory(handle, (void*)(client_DLL + dwEntitylist + i * 0x10), &entity, sizeof(entity), 0);

			if (entity == NULL) {
				continue;
			}

			int entity_Health;
			ReadProcessMemory(handle, (void*)(entity + m_iHealth), &entity_Health, sizeof(entity_Health), 0);

			if (!(entity_Health < 500 && entity_Health > 1)) {
				continue;
			}


			int entity_Team;
			ReadProcessMemory(handle, (void*)(entity + iTeamNum), &entity_Team, sizeof(entity_Team), 0);

			if (local_Team != entity_Team) {
				float entity_pos[3];
				ReadProcessMemory(handle, (void*)(entity + m_vecOrigin), &entity_pos, sizeof(entity_pos), 0);

				float rot = atan2(entity_pos[0] - local_pos[0], entity_pos[1] - local_pos[1]);

				float rot_x = sin(rot + 1.5707 + map(local_rot[1], -180, 180, 0, 6.28318));
				float rot_y = cos(rot + 1.5707 + map(local_rot[1], -180, 180, 0, 6.28318));

				float x1 = 0 + rot_x * 0.3;
				float y1 = 0 + rot_y * (0.3 * 1.6);


				float rot_x2 = sin(rot + 1.5707 + map(local_rot[1] + 4, -180, 180, 0, 6.28318));
				float rot_y2 = cos(rot + 1.5707 + map(local_rot[1] + 4, -180, 180, 0, 6.28318));

				float x2 = 0 + rot_x2 * 0.25;
				float y2 = 0 + rot_y2 * (0.25 * 1.6);


				float rot_x3 = sin(rot + 1.5707 + map(local_rot[1] - 4, -180, 180, 0, 6.28318));
				float rot_y3 = cos(rot + 1.5707 + map(local_rot[1] - 4, -180, 180, 0, 6.28318));

				float x3 = 0 + rot_x3 * 0.25;
				float y3 = 0 + rot_y3 * (0.25 * 1.6);


				glVertex2f(x1, y1);
				glVertex2f(x2, y2);
				glVertex2f(x3, y3);

			}
		}

		glEnd();
	}
};