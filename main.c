// Entonces que pratsito
//saquen a oscaaar del equipo
// CARLOS AAAAAAAAAAA
#include<raylib.h>
#include<raymath.h>
#include<math.h>

struct Player{
    Vector2 position;
    Vector2 velocity;
    float mass;
    float radius;
};

int main(){
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(500, 500, "Hello World");

    struct Player player1 = {
        .position = (Vector2){100,100},
        .velocity = (Vector2){0,0},
        .mass = 50,
        .radius = 20
    };

    const Rectangle platforms[] =  {
        (Rectangle){200, 400, 100, 50},
        (Rectangle){0, 400, 10, 100}
    };

    while(!WindowShouldClose()){
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawCircle(player1.position.x, player1.position.y, player1.radius, RED);
            for(int i=0; i<(sizeof(platforms)/sizeof(Rectangle)); i++){
                DrawRectangleRec(platforms[i], BLACK);
            }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}