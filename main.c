// Entonces que pratsito
#include<stdio.h>
#include<raylib.h>

int main(){
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(500, 500, "Hello World");

    while(!WindowShouldClose()){
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawCircle(50, 50, 20, RED);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}