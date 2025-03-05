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

const Rectangle platforms[] =  {
    (Rectangle){200, -100, 100, 50},
    (Rectangle){0, -100, 10, 100},
    (Rectangle){0, -200, 50, 50}
};

int main(){
    //SetTargetFPS(8);
    InitWindow(500, 500, "Hello World");
    Image imagen_jose_jose = LoadImage("./textures/JoseJose.jpeg");
    Texture2D textura_jose_jose = LoadTextureFromImage(imagen_jose_jose);

    struct Player player1 = {
        .position = (Vector2){100,-200},
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

    bool is_on_ground = false;

    float frame_time_acumulator = 0;
    const float TIME_DELTA_STEP = 0.01;
    bool free_mode = false;

    while(!WindowShouldClose()){
        frame_time_acumulator += GetFrameTime();

        const Rectangle bounding_box = (Rectangle){player1.position.x-player1.radius, player1.position.y-player1.radius, player1.radius*2, player1.radius*2};
        const Vector2 collision_left_1 = (Vector2){player1.position.x-player1.radius, player1.position.y-(player1.radius*0.4)};
        const Vector2 collision_left_2 = (Vector2){player1.position.x-player1.radius, player1.position.y+(player1.radius*0.4)};
        const Vector2 collision_right_1 = (Vector2){player1.position.x+player1.radius, player1.position.y-(player1.radius*0.4)};
        const Vector2 collision_right_2 = (Vector2){player1.position.x+player1.radius, player1.position.y+(player1.radius*0.4)};

        const Vector2 collision_up_1 = (Vector2){player1.position.x-(player1.radius*0.4), player1.position.y-player1.radius};
        const Vector2 collision_up_2 = (Vector2){player1.position.x+(player1.radius*0.4), player1.position.y-player1.radius};
        const Vector2 collision_down_1 = (Vector2){player1.position.x-(player1.radius*0.4), player1.position.y+player1.radius};
        const Vector2 collision_down_2 = (Vector2){player1.position.x+(player1.radius*0.4), player1.position.y+player1.radius};
        const Vector2 collision_down_3 = (Vector2){player1.position.x, player1.position.y+player1.radius};

        Vector2 prev_player_pos = player1.position;
        while(frame_time_acumulator >= TIME_DELTA_STEP && !free_mode){
            frame_time_acumulator -= TIME_DELTA_STEP;
            bool is_touching_ground = false;
            bool is_touching_roof = false;
            bool is_touching_left_wall = false;
            bool is_touching_right_wall = false;

            // Determina colisiones
            if(player1.position.y + player1.radius >= 0){
                is_touching_ground = true;
            }

            for(int i=0; i<sizeof(platforms)/sizeof(Rectangle); i++){
                if(CheckCollisionPointRec(collision_down_1, platforms[i]) || CheckCollisionPointRec(collision_down_2, platforms[i]) || CheckCollisionPointRec(collision_down_3, platforms[i])){
                    float up_lerp = 1;
                    Vector2 lerped_player_pos = player1.position;

                    /*while(up_lerp >= 0){
                        lerped_player_pos = Vector2Lerp(prev_player_pos, player1.position, up_lerp);
                        Vector2 lerped_collision_down_1 = (Vector2){lerped_player_pos.x-(player1.radius*0.4), lerped_player_pos.y+player1.radius};
                        Vector2 lerped_collision_down_2 = (Vector2){lerped_player_pos.x+(player1.radius*0.4), lerped_collision_down_1.y+player1.radius};
                        Vector2 lerped_collision_down_3 = (Vector2){lerped_player_pos.x, lerped_player_pos.y+player1.radius};

                        if(!CheckCollisionPointRec(lerped_collision_down_1, platforms[i]) && !CheckCollisionPointRec(lerped_collision_down_2, platforms[i]) && !CheckCollisionPointRec(lerped_collision_down_3, platforms[i])){
                            player1.position = lerped_player_pos;
                            printf("DO LERPS\n");
                        }
                        
                        up_lerp-=0.25;
                    }*/

                    is_touching_ground = true;
                }

                if(CheckCollisionPointRec(collision_up_1, platforms[i]) || CheckCollisionPointRec(collision_up_2, platforms[i])){
                    is_touching_roof = true;
                }

                if(CheckCollisionPointRec(collision_left_1, platforms[i]) || CheckCollisionPointRec(collision_left_2, platforms[i])){
                    is_touching_left_wall = true;
                }

                if(CheckCollisionPointRec(collision_right_1, platforms[i]) || CheckCollisionPointRec(collision_right_2, platforms[i])){
                    is_touching_right_wall = true;
                }
            }

            // Aplicar fuerzas instantáneas
            if(is_touching_ground && !is_on_ground){
                player1.acceleration.y = 0;
                player1.velocity.y = 0;

                player1.acceleration.x *= 0.5;
            }
            if(is_touching_roof){
                player1.acceleration.y = 0;
                player1.velocity.y = 0;
            }
            if(is_touching_left_wall && player1.velocity.x < 0){
                player1.acceleration.x = 0;
                player1.velocity.x = 0;
            }
            if(is_touching_right_wall && player1.velocity.x > 0){
                player1.acceleration.x = 0;
                player1.velocity.x = 0;
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
            prev_player_pos = player1.position;
            player1.acceleration = Vector2Add(player1.acceleration, Vector2Scale(added_acceleration, TIME_DELTA_STEP));
            player1.velocity = Vector2Add(player1.velocity, Vector2Scale(added_acceleration, TIME_DELTA_STEP));
            player1.position = Vector2Add(player1.position, Vector2Scale(player1.velocity, TIME_DELTA_STEP));

            // TODO: ARREGLAR MOVIMIENTO CAMARA
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
            DrawText(TextFormat("%s (Presiona TAB)", free_mode ? "MODO LIBRE" : "MODO CAMARA NORMAL"), 10, 70, 20, BLACK);
            DrawText(TextFormat("Posicion: (%.2f, %.2f)", player1.position.x, player1.position.y), 10, 85, 20, BLACK);
            DrawText(TextFormat("Acceleracion: (%.2f, %.2f)", player1.acceleration.x, player1.acceleration.y), 10, 100, 20, BLACK);
        BeginMode2D(camera);

            DrawCircleLines(100,-200, 10, RED);
            DrawText("Comienza aqui!!", 120, -200-5, 10, BLACK);
            DrawLineV((Vector2){-1000, 0}, (Vector2){1000, 0}, BLACK);
            DrawLineV((Vector2){0, -1000}, (Vector2){0, 1000}, BLACK);

            
            for(int i=0; i<sizeof(platforms)/sizeof(Rectangle); i++){
                DrawRectangleRec(platforms[i], BLACK);
            }
            
            DrawRectangleLines(player1.position.x-player1.radius, player1.position.y-player1.radius, player1.radius*2, player1.radius*2, RED);
            DrawCircleV(collision_left_1, 2, MAROON);
            DrawCircleV(collision_left_2, 2, MAROON);
            DrawCircleV(collision_right_1, 2, MAROON);
            DrawCircleV(collision_right_2, 2, MAROON);
            
            DrawCircleV(collision_up_1, 2, DARKGREEN);
            DrawCircleV(collision_up_2, 2, DARKGREEN);
            DrawCircleV(collision_down_2, 2, DARKGREEN);
            DrawCircleV(collision_down_1, 2, DARKGREEN);
            DrawCircleV(collision_down_3, 2, DARKGREEN);
            DrawCircle(player1.position.x, player1.position.y, player1.radius, RED);
            EndMode2D();
        EndDrawing();
    }


    UnloadTexture(textura_jose_jose);
    UnloadImage(imagen_jose_jose);
    CloseWindow();

    return 0;
}