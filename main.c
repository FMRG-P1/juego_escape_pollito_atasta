// Entonces que pratsito
//saquen a oscaaar del equipo
// CARLOS AAAAAAAAAAA
#include<raylib.h>
#include<raymath.h>
#include<math.h>
#include<stdio.h>

struct Player{
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;
    float mass;
    float radius;
};

int main(){
    InitWindow(500, 500, "Hello World");

    struct Player player1 = {
        .position = (Vector2){100,200},
        .velocity = (Vector2){100,-50},
        .acceleration = (Vector2){0,0},
        .mass = 50,
        .radius = 20
    };

    const Rectangle platforms[] =  {
        (Rectangle){200, 400, 100, 50},
        (Rectangle){0, 400, 10, 100}
    };

    bool is_on_ground = false;

    while(!WindowShouldClose()){
        const float COMMON_GROUND_FRICTION_COEFFICIENT = 12;
        const float MOVEMENT_JUMP_SENSIBILITY = 20000;
        const float MOVEMENT_DISPLACEMENT_SENSIBILITY = 1000;
        const float FRAME_TIME = GetFrameTime();

        bool is_touching_ground = false;

        Vector2 added_acceleration = {0,0};

        if(player1.position.y + player1.radius >= GetScreenHeight()){
            is_touching_ground = true;
        }

        if(!is_touching_ground){
            added_acceleration.y += 9.8*player1.mass; // Gravity
        }
        if(is_touching_ground && !is_on_ground){
            player1.acceleration.y = 0;
            player1.velocity.y = 0;
        }
        if(is_touching_ground){
            if(IsKeyDown(KEY_SPACE)){
                added_acceleration.y += -MOVEMENT_JUMP_SENSIBILITY*player1.mass;
                is_touching_ground = false;
            }
        }

        is_on_ground = is_touching_ground;

        if(IsKeyDown(KEY_LEFT)){ added_acceleration.x += -MOVEMENT_DISPLACEMENT_SENSIBILITY; }
        if(IsKeyDown(KEY_RIGHT)){ added_acceleration.x += MOVEMENT_DISPLACEMENT_SENSIBILITY; }
          

        player1.acceleration = Vector2Add(player1.acceleration, Vector2Scale(added_acceleration, FRAME_TIME));
        player1.velocity = Vector2Add(player1.velocity, Vector2Scale(added_acceleration, FRAME_TIME));
        player1.position = Vector2Add(player1.position, Vector2Scale(player1.velocity, FRAME_TIME));

        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawCircle(player1.position.x, player1.position.y, player1.radius, RED);
            DrawLineV(player1.position, (Vector2){player1.position.x + player1.acceleration.x, player1.position.y + player1.acceleration.y}, BLUE);

            DrawText(TextFormat("Acceleration (%.2f, %.2f)", player1.acceleration.x, player1.acceleration.y), 100, 100, 20, BLACK);

            for(int i=0; i<(sizeof(platforms)/sizeof(Rectangle)); i++){
                DrawRectangleRec(platforms[i], BLACK);
            }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}