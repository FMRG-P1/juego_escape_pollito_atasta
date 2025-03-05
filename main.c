// Entonces que pratsito
//saquen a oscaaar del equipo
// CARLOS AAAAAAAAAAA
#include<raylib.h>
#include<raymath.h>
#include<math.h>
#include<stdio.h>

#define METER_UNIT 100

struct Player{
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;
    float mass;
    float radius;
};

int main(){
    SetTargetFPS(12);
    InitWindow(500, 500, "Hello World");
    Image imagen_jose_jose = LoadImage("./textures/JoseJose.jpeg");
    Texture2D textura_jose_jose = LoadTextureFromImage(imagen_jose_jose);

    struct Player player1 = {
        .position = (Vector2){100,200},
        .velocity = (Vector2){0,0},
        .acceleration = (Vector2){0,0},
        .mass = 50,
        .radius = 20
    };

    const Rectangle platforms[] =  {
        (Rectangle){200, 400, 100, 50},
        (Rectangle){0, 400, 10, 100}
    };

    bool is_on_ground = false;

    float acc = 0;
    float TIME_DELTA_STEP = 0.01;

#if 1
    while(!WindowShouldClose()){
        acc += GetFrameTime();

        while(acc >= TIME_DELTA_STEP){
            acc -= TIME_DELTA_STEP;
            bool is_touching_ground = false;

            if(player1.position.y + player1.radius >= GetScreenHeight()){
                is_touching_ground = true;
            }

            if(is_touching_ground && !is_on_ground){
                player1.acceleration.y = 0;
                player1.velocity.y = 0;
            }
            if(is_touching_ground){
                if(IsKeyDown(KEY_SPACE)){
                    player1.velocity.y -= 5*METER_UNIT;
                    is_touching_ground = false;
                }
            }
    
            is_on_ground = is_touching_ground;


            Vector2 added_acceleration = {0,0};
            if(!is_touching_ground){
                added_acceleration.y += 9.8*player1.mass;
            }

            {
                float movement_x_factor = is_touching_ground ? 1 : 0.7;
                
                if(IsKeyDown(KEY_LEFT)){ added_acceleration.x += -2*METER_UNIT*movement_x_factor; }
                if(IsKeyDown(KEY_RIGHT)){ added_acceleration.x += 2*METER_UNIT*movement_x_factor; }
            }

            player1.acceleration = Vector2Add(player1.acceleration, Vector2Scale(added_acceleration, TIME_DELTA_STEP));
            player1.velocity = Vector2Add(player1.velocity, Vector2Scale(added_acceleration, TIME_DELTA_STEP));
            player1.position = Vector2Add(player1.position, Vector2Scale(player1.velocity, TIME_DELTA_STEP));
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText(TextFormat("Acceleration: (%.2f, %.2f)", player1.acceleration.x, player1.acceleration.y), 100, 100, 20, BLACK);
            DrawLineEx((Vector2){100, 100}, (Vector2){100+METER_UNIT,100}, 5, RED);
            DrawCircle(player1.position.x, player1.position.y, player1.radius, RED);
        EndDrawing();
    }
#else
    while(!WindowShouldClose()){
        const float COMMON_GROUND_FRICTION_COEFFICIENT = 12;
        const float MOVEMENT_JUMP_SENSIBILITY = 1500;
        const float MOVEMENT_DISPLACEMENT_SENSIBILITY = 800;
        const float FRAME_TIME = GetFrameTime();

        bool is_touching_ground = false;

        Vector2 added_acceleration = {0,0};

        if(player1.position.y + player1.radius >= GetScreenHeight()){
            is_touching_ground = true;
        }

        for(int i=0; i<(sizeof(platforms)/sizeof(Rectangle)); i++){
            if(CheckCollisionCircleRec((Vector2){player1.position.x, player1.position.y}, player1.radius, platforms[i])){
                is_touching_ground = true;
            }
        }

        if(!is_touching_ground){
            if(FRAME_TIME < 0.5){
                added_acceleration.y += 9.8*player1.mass; // Gravity
            }
        }
        if(is_touching_ground && !is_on_ground){
            player1.acceleration.y = 0;
            player1.velocity.y = 0;
        }
        if(is_touching_ground){
            if(IsKeyPressed(KEY_SPACE)){
                player1.acceleration.y += MOVEMENT_JUMP_SENSIBILITY/player1.mass;
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
            DrawTextureEx(textura_jose_jose, (Vector2){player1.position.x-player1.radius, player1.position.y-player1.radius}, 0, 0.01, RAYWHITE);
            DrawLineV(player1.position, (Vector2){player1.position.x + player1.acceleration.x, player1.position.y + player1.acceleration.y}, BLUE);

            DrawText(TextFormat("Acceleration (%.2f, %.2f)", player1.acceleration.x, player1.acceleration.y), 100, 100, 20, BLACK);
            DrawText(TextFormat("Position (%.2f, %.2f)", player1.position.x, player1.position.y), 100, 125, 20, BLACK);

            for(int i=0; i<(sizeof(platforms)/sizeof(Rectangle)); i++){
                DrawRectangleRec(platforms[i], BLACK);
            }
        EndDrawing();
    }
#endif

    UnloadTexture(textura_jose_jose);
    UnloadImage(imagen_jose_jose);
    CloseWindow();

    return 0;
}