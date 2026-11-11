#include "raylib.h"

int main(void) {
    InitWindow(960, 540, "Último Turno");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}