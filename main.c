// Entonces que pratsito
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
        if(IsKeyPressed(KEY_F5)){
            player1.position = (Vector2){100,100};
            player1.velocity = (Vector2){0,0};
        }

        const float FRAME_TIME = GetFrameTime();
        Vector2 aplied_acceleration;
        bool is_on_ground = false;

        const Vector2 collision_0deg = (Vector2){player1.position.x+player1.radius, player1.position.y};
        const Vector2 collision_90deg = (Vector2){player1.position.x-player1.radius, player1.position.y};
        const Vector2 collision_180deg = (Vector2){player1.position.x-player1.radius, player1.position.y};
        const Vector2 collision_270deg = (Vector2){player1.position.x+player1.radius, player1.position.y};
        
        aplied_acceleration = (Vector2){0,9.8*player1.mass*FRAME_TIME};

        if(player1.position.y+20 > GetScreenHeight()){
            is_on_ground = true;
            aplied_acceleration.y = 0;
            player1.velocity.y = 0;
        }

        for(int i=0; i<(sizeof(platforms)/sizeof(Rectangle)); i++){
            if(CheckCollisionPointRec(collision_90deg, platforms[i])){
                aplied_acceleration.x = 0;
                player1.velocity.x = 0;
                player1.position.x += 1;
                is_on_ground = true;
            }

            if(CheckCollisionPointRec(collision_0deg, platforms[i])){
                aplied_acceleration.x = 0;
                player1.velocity.x = 0;
                player1.position.x += -1;
                is_on_ground = true;
            }

            if(CheckCollisionPointRec(collision_270deg, platforms[i])){
                aplied_acceleration.y = 0;
                player1.velocity.y = 0;
                player1.position.y += -1;
                is_on_ground = true;
            }
        }

        if(is_on_ground){
            if(IsKeyDown(KEY_SPACE)){ aplied_acceleration.y += -300; }
        }

        if(IsKeyDown(KEY_LEFT)){ aplied_acceleration.x += -200*FRAME_TIME; }
        if(IsKeyDown(KEY_RIGHT)){ aplied_acceleration.x += 200*FRAME_TIME; }
        
        player1.velocity = Vector2Add(player1.velocity, aplied_acceleration);
        player1.position = Vector2Add(player1.position, Vector2Scale(player1.velocity, FRAME_TIME));

        BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawCircle(player1.position.x, player1.position.y, player1.radius, RED);
            DrawCircleV(collision_0deg, 2, MAROON);
            DrawCircleV(collision_90deg, 2, MAROON);
            DrawCircleV(collision_180deg, 2, MAROON);
            DrawCircleV(collision_270deg, 2, MAROON);

            for(int i=0; i<(sizeof(platforms)/sizeof(Rectangle)); i++){
                DrawRectangleRec(platforms[i], BLACK);
            }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}