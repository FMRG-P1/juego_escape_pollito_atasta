// Entonces que pratsito
#include<raylib.h>
#include<raymath.h>
#include<math.h>

struct Player{
    Vector2 position;
    Vector2 velocity;
    float mass;
};

int main(){
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(500, 500, "Hello World");

    struct Player player1 = {
        .position = (Vector2){100,100},
        .velocity = (Vector2){0,0},
        .mass = 50
    };

    Rectangle platform = (Rectangle){200, 400, 100, 50};

    while(!WindowShouldClose()){
        const float FRAME_TIME = GetFrameTime();
        bool is_on_ground = player1.position.y+20 > GetScreenHeight();

        if(IsKeyPressed(KEY_F5)){
            player1.position = (Vector2){100,100};
            player1.velocity = (Vector2){0,0};
        }
        
        Vector2 aplied_acceleration;
        aplied_acceleration = (Vector2){0,9.8*player1.mass*FRAME_TIME};

        if(is_on_ground){
            aplied_acceleration.y += 0;
            player1.velocity.y = 0;

            if(IsKeyReleased(KEY_SPACE)){
                aplied_acceleration.y += -300;
            }
        }

        if(IsKeyDown(KEY_LEFT)){
            aplied_acceleration.x += -200*FRAME_TIME;
        }
        if(IsKeyDown(KEY_RIGHT)){
            aplied_acceleration.x += 200*FRAME_TIME;
        }
        
        player1.velocity = Vector2Add(player1.velocity, aplied_acceleration);
        player1.position = Vector2Add(player1.position, Vector2Scale(player1.velocity, FRAME_TIME));

        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawCircle(player1.position.x, player1.position.y, 20, RED);
            DrawRectangle(200, 400, 100, 50, RED);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}