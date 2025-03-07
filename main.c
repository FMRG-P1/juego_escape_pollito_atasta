#include<raylib.h>
#include<raymath.h>
#include<math.h>
#include<stdio.h>

#define METER_UNIT 100


enum GameLevel_Kind{
    GAME_LEVEL_TEST
};

enum PlayerState_Kind{
    PLAYER_WALINKG,
    PLAYER_FALLING,
    PLAYER_IDDLE
};

struct Player {
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;
    float mass;
    float radius;

    enum PlayerState_Kind state;
};

void level_test_draw(struct Player* player_current, Camera2D* camera, const float TIME_DELTA_STEP);
bool level_test_collision(struct Player* player_current);

int main(){
    InitWindow(500, 500, "Pollos Pomoca");

    enum GameLevel_Kind game_scenario = GAME_LEVEL_TEST;
    struct Player player1 = {
        .position = (Vector2){100,-200},
        .velocity = (Vector2){0,0},
        .acceleration = (Vector2){0,0},
        .mass = 50,
        .radius = 20,
        .state = PLAYER_IDDLE,
    };

    struct Player player1_prev = {0};

    Camera2D camera;
    camera.target = (Vector2){ player1.position.x, player1.position.y };
    camera.offset = (Vector2){ GetScreenWidth()/2.0f, GetScreenHeight()/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;


    float frame_time_acumulator = 0;
    const float TIME_DELTA_STEP = 0.01;

    while(!WindowShouldClose()){
        frame_time_acumulator += GetFrameTime();

        while(frame_time_acumulator >= TIME_DELTA_STEP){
            frame_time_acumulator -= TIME_DELTA_STEP;

            if(level_test_collision(&player1)){
                player1.position = Vector2Lerp(player1.position, player1_prev.position, 0.5*TIME_DELTA_STEP); // Aproximacion
            }

            Vector2 acumulated_impulse = {0,0};
            if(player1.state == PLAYER_FALLING){
                acumulated_impulse.y += 9.8*player1.mass;
            }else{                
                const float friction_coefficient = 0.1;
                if(player1.velocity.x < 1 || player1.velocity.x > 1){
                    acumulated_impulse.x += -friction_coefficient*9.8*player1.acceleration.x;
                }else{
                    player1.acceleration.x = 0;
                }

                if(IsKeyDown(KEY_SPACE)){
                    player1.velocity.y -= 5*METER_UNIT;
                    player1.state = PLAYER_FALLING;
                }
            }
            {
                if((IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT)) && player1.state == PLAYER_IDDLE){
                    player1.state = PLAYER_WALINKG;
                }
                
                float movement_x_factor = player1.state == PLAYER_FALLING ? 0.7 : 1;
                if(IsKeyDown(KEY_LEFT)){ acumulated_impulse.x += -2*METER_UNIT*movement_x_factor; }
                if(IsKeyDown(KEY_RIGHT)){ acumulated_impulse.x += 2*METER_UNIT*movement_x_factor; }
            }

            player1_prev = player1;
            player1.acceleration = Vector2Add(player1.acceleration, Vector2Scale(acumulated_impulse, TIME_DELTA_STEP));
            // NO! NO! NO ARREGLES ESTA LINEA, ESTA BIEN, NO!
            player1.velocity = Vector2Add(player1.velocity, Vector2Scale(acumulated_impulse, TIME_DELTA_STEP));
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

            // CHECK COLLISION
            // STATE PLAYER
            // STATE ...
            // CAMERA
            // SIMULATE PHYSICS
            // RENDER
        }

        
        level_test_draw(&player1, &camera, TIME_DELTA_STEP);
    }

    CloseWindow();
}

bool collision_player_rectangle_simulate(struct Player* player, const Rectangle rectangle){
    const Rectangle bounding_box = (Rectangle){player->position.x-player->radius, player->position.y-player->radius, player->radius*2, player->radius*2};
    const Vector2 collision_left_1 = (Vector2){player->position.x-player->radius, player->position.y-(player->radius*0.4)};
    const Vector2 collision_left_2 = (Vector2){player->position.x-player->radius, player->position.y+(player->radius*0.4)};
    const Vector2 collision_right_1 = (Vector2){player->position.x+player->radius, player->position.y-(player->radius*0.4)};
    const Vector2 collision_right_2 = (Vector2){player->position.x+player->radius, player->position.y+(player->radius*0.4)};

    const Vector2 collision_up_1 = (Vector2){player->position.x-(player->radius*0.4), player->position.y-player->radius};
    const Vector2 collision_up_2 = (Vector2){player->position.x+(player->radius*0.4), player->position.y-player->radius};
    const Vector2 collision_down_1 = (Vector2){player->position.x-(player->radius*0.4), player->position.y+player->radius};
    const Vector2 collision_down_2 = (Vector2){player->position.x+(player->radius*0.4), player->position.y+player->radius};
    const Vector2 collision_down_3 = (Vector2){player->position.x, player->position.y+player->radius};
    bool has_collided = false;

    if(CheckCollisionPointRec(collision_down_1, rectangle) || CheckCollisionPointRec(collision_down_2, rectangle) || CheckCollisionPointRec(collision_down_3, rectangle)){
        if(player->state == PLAYER_FALLING){
            player->acceleration.y = 0;
            player->velocity.y = 0;
            player->acceleration.x *= 0.5;
        }
        player->state = PLAYER_IDDLE;
        has_collided = true;
    }

    if(CheckCollisionPointRec(collision_up_1, rectangle) || CheckCollisionPointRec(collision_up_2, rectangle)){
        player->acceleration.y = 0;
        player->velocity.y = 0;

        player->state = PLAYER_IDDLE;
        has_collided = true;
    }

    if(CheckCollisionPointRec(collision_left_1, rectangle) || CheckCollisionPointRec(collision_left_2, rectangle)){
        if(player->velocity.x < 0){
            player->acceleration.x = 0;
            player->velocity.x = 0;

            player->state = PLAYER_IDDLE;
            has_collided = true;
        }
    }

    if(CheckCollisionPointRec(collision_right_1, rectangle) || CheckCollisionPointRec(collision_right_2, rectangle)){
        if(player->velocity.x > 0){
            player->acceleration.x = 0;
            player->velocity.x = 0;

            player->state = PLAYER_IDDLE;
            has_collided = true;
        }
    }

    if(!has_collided){
        player->state = PLAYER_FALLING;
    }

    return has_collided;
}

bool collision_player_floor_simulate(struct Player* player, float floor_y){
    if(player->position.y + player->radius >= floor_y){
        player->acceleration.y = 0;
        player->velocity.y = 0;

        player->state = PLAYER_IDDLE;
        return true;
    }

    return false;
}

bool level_test_collision(struct Player* player_current){
    const Rectangle platforms[] =  {
        (Rectangle){200, -100, 100, 50},
        (Rectangle){0, -100, 10, 100},
        (Rectangle){0, -200, 50, 50}
    };

    bool has_collided = false;
    // COLLISIONES
    for(int i=0; i<sizeof(platforms)/sizeof(Rectangle); i++){
        if(collision_player_rectangle_simulate(player_current, platforms[i])){
            has_collided = true;
        }
    }

    if(collision_player_floor_simulate(player_current, 0)){
        has_collided = true;
    }

    return has_collided;
}

void level_test_draw(struct Player* player_current, Camera2D* camera, const float TIME_DELTA_STEP){
    const Rectangle platforms[] =  {
        (Rectangle){200, -100, 100, 50},
        (Rectangle){0, -100, 10, 100},
        (Rectangle){0, -200, 50, 50}
    };

    // DIBUJO 
    BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText(TextFormat("Posicion: (%.2f, %.2f)", player_current->position.x, player_current->position.y), 10, 85, 20, BLACK);
        DrawText(TextFormat("Acceleracion: (%.2f, %.2f)", player_current->acceleration.x, player_current->acceleration.y), 10, 100, 20, BLACK);
    BeginMode2D(*camera);
        DrawLineV((Vector2){-1000, 0}, (Vector2){1000, 0}, BLACK);
        DrawLineV((Vector2){0, -1000}, (Vector2){0, 1000}, BLACK);
        for(int i=0; i<sizeof(platforms)/sizeof(Rectangle); i++){
            DrawRectangleRec(platforms[i], BLACK);
        }
        DrawCircle(player_current->position.x, player_current->position.y, player_current->radius, RED);
    EndMode2D();
    EndDrawing();
}