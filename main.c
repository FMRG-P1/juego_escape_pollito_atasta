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

    Camera2D old_player_camera;
    Camera2D camera;
    camera.target = (Vector2){ player1.position.x, player1.position.y };
    camera.offset = (Vector2){ GetScreenWidth()/2.0f, GetScreenHeight()/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    const Rectangle platforms[] =  {
        (Rectangle){200, 400, 100, 50},
        (Rectangle){0, 400, 10, 100}
    };

    bool is_on_ground = false;

    float frame_time_acumulator = 0;
    const float TIME_DELTA_STEP = 0.01;
    bool free_mode = false;

    while(!WindowShouldClose()){
        frame_time_acumulator += GetFrameTime();

        while(frame_time_acumulator >= TIME_DELTA_STEP && !free_mode){
            frame_time_acumulator -= TIME_DELTA_STEP;
            bool is_touching_ground = false;

            // Determina colisiones
            if(player1.position.y + player1.radius >= GetScreenHeight()){
                is_touching_ground = true;
            }

            // Aplicar fuerzas instantáneas
            if(is_touching_ground && !is_on_ground){
                player1.acceleration.y = 0;
                player1.velocity.y = 0;

                player1.acceleration.x *= 0.5;
            }
            if(is_touching_ground){
                if(IsKeyDown(KEY_SPACE)){
                    player1.velocity.y -= 5*METER_UNIT;
                    player1.acceleration.x *= 1.5;
                    is_touching_ground = false;
                }
            }
    
            is_on_ground = is_touching_ground;

            // Aplicar aceleraciones
            Vector2 added_acceleration = {0,0};
            if(!is_touching_ground){
                added_acceleration.y += 9.8*player1.mass;
            } else {
                const float friction_coefficient = 0.1;
                if(player1.velocity.x < 1 || player1.velocity.x > 1){
                    added_acceleration.x += -friction_coefficient*9.8*player1.acceleration.x;
                }else{
                    player1.acceleration.x = 0;
                }
            }

            {
                float movement_x_factor = is_touching_ground ? 1 : 0.7;
                
                if(IsKeyDown(KEY_LEFT)){ added_acceleration.x += -2*METER_UNIT*movement_x_factor; }
                if(IsKeyDown(KEY_RIGHT)){ added_acceleration.x += 2*METER_UNIT*movement_x_factor; }
            }

            // Computar movimiento
            player1.acceleration = Vector2Add(player1.acceleration, Vector2Scale(added_acceleration, TIME_DELTA_STEP));
            player1.velocity = Vector2Add(player1.velocity, Vector2Scale(added_acceleration, TIME_DELTA_STEP));
            player1.position = Vector2Add(player1.position, Vector2Scale(player1.velocity, TIME_DELTA_STEP));

            if(GetWorldToScreen2D(player1.position, camera).y > GetScreenHeight()+20){
                camera.target.y += GetWorldToScreen2D(player1.position, camera).y+player1.radius -  GetScreenHeight()+10;
            }
            if(GetWorldToScreen2D(player1.position, camera).x < 0+20 && player1.velocity.x < 0){
                camera.target.x += - 20;
            }

            if(GetWorldToScreen2D(player1.position, camera).x > GetScreenWidth()-20 && player1.velocity.x > 0){
                camera.target.x += player1.radius + 20;
            }
    
        }

        if(free_mode){
            if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
                Vector2 mouse_delta = GetMouseDelta();
                camera.offset = Vector2Add(camera.offset, mouse_delta);
            }

            float mouse_wheel = GetMouseWheelMove();
            camera.zoom += mouse_wheel/10;
        }
        if(IsKeyPressed(KEY_TAB)){
            if(!free_mode){
                old_player_camera = camera;
            }
            if(free_mode){
                camera = old_player_camera;
            }
            free_mode = !free_mode;
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText(TextFormat("%s (Presiona TAB)", free_mode ? "MODO LIBRE" : "MODO CAMARA NORMAL"), 100, 70, 20, BLACK);
            DrawText(TextFormat("Posicion: (%.2f, %.2f)", player1.position.x, player1.position.y), 100, 85, 20, BLACK);
            DrawText(TextFormat("Acceleracion: (%.2f, %.2f)", player1.acceleration.x, player1.acceleration.y), 100, 100, 20, BLACK);
        BeginMode2D(camera);
            
            for(int i=0; i<sizeof(platforms)/sizeof(Rectangle); i++){
                DrawRectangleRec(platforms[i], BLACK);
            }

            DrawCircle(player1.position.x, player1.position.y, player1.radius, RED);
        EndMode2D();
        EndDrawing();
    }


    UnloadTexture(textura_jose_jose);
    UnloadImage(imagen_jose_jose);
    CloseWindow();

    return 0;
}