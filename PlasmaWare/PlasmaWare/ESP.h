#pragma once

class ESP {
    struct Vec2 {
        float X;
        float Y;
    };

    struct Vec3 {
        float X;
        float Y;
        float Z;
    };

    struct Matrix {
        float VMatrix[16];
    };

    void ConvertToRange(Vec2& Point) {
        Point.X /= 2560;
        Point.X *= 2.0f;
        Point.X -= 1.0f;

        Point.Y /= 1440;
        Point.Y *= 2.0f;
        Point.Y -= 1.0f;
    }

    bool WorldToScreen(const Vec3& VecOrigin, Vec2& VecScreen, float* Matrix) {
        VecScreen.X = VecOrigin.X * Matrix[0] + VecOrigin.Y * Matrix[1] + VecOrigin.Z * Matrix[2] + Matrix[3];
        VecScreen.Y = VecOrigin.X * Matrix[4] + VecOrigin.Y * Matrix[5] + VecOrigin.Z * Matrix[6] + Matrix[7];
        float W = VecOrigin.X * Matrix[12] + VecOrigin.Y * Matrix[13] + VecOrigin.Z * Matrix[14] + Matrix[15];

        if (W < 0.01f) {
            return false;
        }

        Vec2 NDC;
        NDC.X = VecScreen.X / W;
        NDC.Y = VecScreen.Y / W;

        VecScreen.X = (2560 / 2 * NDC.X) + (NDC.X + 2560 / 2);
        VecScreen.Y = (1440 / 2 * NDC.Y) + (NDC.Y + 1440 / 2);

        ConvertToRange(VecScreen);

        return true;
    }

public:
    bool Esp_box = true;
    bool Esp_line = true;
    bool Esp_distance = 0;

    void loop(HANDLE process) {
        glBegin(GL_LINES);

        uintptr_t Player;
        ReadProcessMemory(process, (LPCVOID)(client_DLL + dwlocalplayer), &Player, sizeof(Player), NULL);

        Vec3 PlayerLocation;
        ReadProcessMemory(process, (LPCVOID)(Player + m_vecOrigin), &PlayerLocation, sizeof(PlayerLocation), NULL);


        Matrix ViewMatrix;
        uintptr_t EntityList;
        uintptr_t Localplayer;
        uintptr_t Clientstate;

        ReadProcessMemory(process, (LPCVOID)(engine_DLL + dwClientState), &Clientstate, sizeof(Clientstate), NULL);
        ReadProcessMemory(process, (LPCVOID)(Clientstate + dwViewMatrix), &ViewMatrix, sizeof(ViewMatrix), NULL);
        ReadProcessMemory(process, (LPCVOID)(client_DLL + dwEntitylist), &EntityList, sizeof(EntityList), NULL);
        ReadProcessMemory(process, (LPCVOID)(client_DLL + dwlocalplayer), &Localplayer, sizeof(Localplayer), NULL);

        int Localplayer_Team;
        ReadProcessMemory(process, (LPCVOID)(Localplayer + iTeamNum), &Localplayer_Team, sizeof(Localplayer_Team), NULL);



        for (int i = 0; i < 80; i++) {
            uintptr_t Entity;
            ReadProcessMemory(process, (LPCVOID)(client_DLL + dwEntitylist + i * 0x10), &Entity, sizeof(Entity), NULL);

            if (Entity == NULL) {
                continue;
            }


            int entity_Health;
            ReadProcessMemory(process, (void*)(Entity + m_iHealth), &entity_Health, sizeof(entity_Health), 0);

            if (!(entity_Health < 300 && entity_Health > 1)) {
                continue;
            }

            int entity_Team;
            ReadProcessMemory(process, (void*)(Entity + iTeamNum), &entity_Team, sizeof(entity_Team), 0);

            if (Localplayer_Team == entity_Team) {
                continue;
            }


            glColor3f(1, 0, 0);
                

            Vec3 EntityLocationDown;
            ReadProcessMemory(process, (LPCVOID)(Entity + m_vecOrigin), &EntityLocationDown, sizeof(EntityLocationDown), NULL);

            float dis = hypot(EntityLocationDown.X - PlayerLocation.X, EntityLocationDown.Y - PlayerLocation.Y);

            if (dis < Esp_distance) {
                continue;
            }

            Vec2 ScreenCoords1;
            WorldToScreen(EntityLocationDown, ScreenCoords1, ViewMatrix.VMatrix);

            Vec3 EntityLocationTop = EntityLocationDown;
            EntityLocationTop.Z += 80;


            Vec2 ScreenCoords2;
            WorldToScreen(EntityLocationTop, ScreenCoords2, ViewMatrix.VMatrix);
            

            float w = (ScreenCoords1.Y - ScreenCoords2.Y) / 7;

            if (Esp_line) {
                glVertex2f(0, 0);
                glVertex2f(ScreenCoords1.X, ScreenCoords1.Y);
            }

            if (Esp_box) {
                glVertex2f(ScreenCoords1.X, ScreenCoords1.Y);
                glVertex2f(ScreenCoords1.X + w, ScreenCoords1.Y);

                glVertex2f(ScreenCoords1.X, ScreenCoords1.Y);
                glVertex2f(ScreenCoords1.X - w, ScreenCoords1.Y);

                // draw line at the top

                glVertex2f(ScreenCoords2.X, ScreenCoords2.Y);
                glVertex2f(ScreenCoords2.X + w, ScreenCoords2.Y);

                glVertex2f(ScreenCoords2.X, ScreenCoords2.Y);
                glVertex2f(ScreenCoords2.X - w, ScreenCoords2.Y);

                // draw line on rigth side

                glVertex2f(ScreenCoords1.X + w, ScreenCoords1.Y);
                glVertex2f(ScreenCoords2.X + w, ScreenCoords2.Y);

                // draw line on left side

                glVertex2f(ScreenCoords1.X - w, ScreenCoords1.Y);
                glVertex2f(ScreenCoords2.X - w, ScreenCoords2.Y);
            }
        }

        glEnd();
    }
};