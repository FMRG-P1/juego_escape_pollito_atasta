#include<raylib.h>
#include<raymath.h>
#include<math.h>
#include<stdio.h>

#define METER_UNIT 100

struct CollisionFlags{
    bool is_on_wall;
    bool is_on_floor;
    bool is_on_ceiling;
};

enum PlayerState_Kind{
    PLAYER_FALLING,
    PLAYER_IDDLE,
    PLAYER_LANDING,
    PLAYER_WALKING,
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
struct CollisionFlags level_test_collision(struct Player* player_current);

Texture2D g_texture_jose_jose;


int main(){
    InitWindow(500, 500, "Pollos Pomoca");

    // Declarar texturas
    Image image_jose_jose = LoadImage("./textures/JoseJose.jpeg");
    g_texture_jose_jose = LoadTextureFromImage(image_jose_jose);

    const struct Player player_initial_state = {
        .position = (Vector2){100,-200},
        .velocity = (Vector2){0,0},
        .acceleration = (Vector2){0,0},
        .mass = 50,
        .radius = 20,
        .state = PLAYER_IDDLE,
    };

    struct Player player1 = player_initial_state;
    struct Player player1_prev = {0};

    Camera2D camera;
    camera.target = (Vector2){ player1.position.x, player1.position.y };
    camera.offset = (Vector2){ GetScreenWidth()/2.0f, GetScreenHeight()/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    float frame_time_acumulator = 0;
    const float TIME_DELTA_STEP = 0.01;
    bool free_mode = false;

    while(!WindowShouldClose()){
        frame_time_acumulator += GetFrameTime();

        while(frame_time_acumulator >= TIME_DELTA_STEP && !free_mode){
            frame_time_acumulator -= TIME_DELTA_STEP;

            struct CollisionFlags collision_flags = level_test_collision(&player1);

            // STATE MACHINE
            switch(player1.state){
                case PLAYER_FALLING:
                    if(collision_flags.is_on_floor){
                        player1.state = PLAYER_LANDING;
                    }
                break;
                case PLAYER_LANDING:
                    if(collision_flags.is_on_floor){
                        player1.state = PLAYER_IDDLE;
                    }
                break;
                case PLAYER_IDDLE:
                    if(!collision_flags.is_on_floor){
                        player1.state = PLAYER_FALLING;
                    }

                    if(player1.velocity.x != 0){
                        player1.state = PLAYER_WALKING;
                    }
                break;
                case PLAYER_WALKING:
                    if(!collision_flags.is_on_floor){
                        player1.state = PLAYER_FALLING;
                    }

                    if(player1.velocity.x == 0){
                        player1.state = PLAYER_IDDLE;
                    }
                break;
            }

            Vector2 acumulated_impulse = {0,0};
            {                
                float movement_x_factor = player1.state == PLAYER_FALLING ? 0.7 : 1;
                if(IsKeyDown(KEY_LEFT)){ acumulated_impulse.x += -2*METER_UNIT*movement_x_factor; }
                if(IsKeyDown(KEY_RIGHT)){ acumulated_impulse.x += 2*METER_UNIT*movement_x_factor; }
            }

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
                    
                    // TODO: Refactor if statement
                    if(
                        player1.state == PLAYER_LANDING && 
                        !(IsKeyDown(KEY_LEFT) && IsKeyDown(KEY_RIGHT)) &&
                        (
                            (player1.acceleration.x < 0 && IsKeyDown(KEY_RIGHT)) ||
                            (player1.acceleration.x > 0 && IsKeyDown(KEY_LEFT))
                        )
                    ){
                        player1.velocity.x = -player1.velocity.x/4;
                        player1.acceleration.x = -player1.acceleration.x/4;
                        printf("BUNYHOP\n");
                    } else {
                        player1.velocity.x = player1.velocity.x*0.8;
                    }

                    player1.state = PLAYER_FALLING;
                }
            }

            player1_prev = player1;
            player1.acceleration = Vector2Add(player1.acceleration, Vector2Scale(acumulated_impulse, TIME_DELTA_STEP));
            // NO! NO! NO ARREGLES ESTA LINEA, ESTA BIEN, NO!
            player1.velocity = Vector2Add(player1.velocity, Vector2Scale(acumulated_impulse, TIME_DELTA_STEP));
            player1.position = Vector2Add(player1.position, Vector2Scale(player1.velocity, TIME_DELTA_STEP));

            if(GetWorldToScreen2D(player1.position, camera).y < 0+20){
                camera.target.y += -20;
            }
            if(GetWorldToScreen2D(player1.position, camera).y > GetScreenHeight()-20){
                camera.target.y += 20;
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


        if(free_mode){
            if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
                camera.offset  = Vector2Add(camera.offset, GetMouseDelta());
            }

            camera.zoom += GetMouseWheelMove()/10;

            if(IsKeyReleased(KEY_TAB)) {
                free_mode = false;
                camera.target = (Vector2){ player1.position.x, player1.position.y };
                camera.offset = (Vector2){ GetScreenWidth()/2.0f, GetScreenHeight()/2.0f };
                camera.rotation = 0.0f;
                camera.zoom = 1.0f;
                player1 = player_initial_state;
            }
        } else {
            if(IsKeyReleased(KEY_TAB)){
                free_mode = true;
            }
        }

        level_test_draw(&player1, &camera, TIME_DELTA_STEP);
    }

    UnloadTexture(g_texture_jose_jose);
    UnloadImage(image_jose_jose);
    CloseWindow();
}

struct CollisionFlags collision_player_rectangle_simulate(struct Player* player, const Rectangle rectangle){
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

    struct CollisionFlags flags = {false, false, false};

    if(CheckCollisionPointRec(collision_down_1, rectangle) || CheckCollisionPointRec(collision_down_2, rectangle) || CheckCollisionPointRec(collision_down_3, rectangle)){
        if(player->state == PLAYER_FALLING){
            player->acceleration.x *= 0.5;
            player->acceleration.y = 0;
            player->velocity.y = 0;
        }
        flags.is_on_floor = true;
    }

    if(CheckCollisionPointRec(collision_up_1, rectangle) || CheckCollisionPointRec(collision_up_2, rectangle)){
        player->acceleration.y = 0;
        player->velocity.y = 0;
        flags.is_on_ceiling = true;
    }

    if(CheckCollisionPointRec(collision_left_1, rectangle) || CheckCollisionPointRec(collision_left_2, rectangle)){
        if(player->velocity.x < 0){
            player->acceleration.x = 0;
            player->velocity.x = 0;
        }
        flags.is_on_wall = true;
    }

    if(CheckCollisionPointRec(collision_right_1, rectangle) || CheckCollisionPointRec(collision_right_2, rectangle)){
        if(player->velocity.x > 0){
            player->acceleration.x = 0;
            player->velocity.x = 0;
        }
        flags.is_on_wall = true;
    }

    return flags;
}

bool collision_player_floor_simulate(struct Player* player, float floor_y){
    if(player->position.y + player->radius >= floor_y){
        if(player->state == PLAYER_FALLING){
            player->acceleration.y = 0;
            player->velocity.y = 0;
        }
        return true;
    }

    return false;
}

struct CollisionFlags level_test_collision(struct Player* player_current){
    const Rectangle platforms_jump[] = {
        (Rectangle){100, -10, 50, 10},
        (Rectangle){150, -50, 50, 10}
    };

    const Rectangle platforms[] =  {
        (Rectangle){0, -100, 10, 100},
        (Rectangle){0, -200, 50, 40},
    };

    
    struct CollisionFlags flags = {false, false, false};
    flags.is_on_floor |= collision_player_floor_simulate(player_current, 0);

    for(int i=0; i<sizeof(platforms)/sizeof(Rectangle); i++){
        const struct CollisionFlags temp = collision_player_rectangle_simulate(player_current, platforms[i]);
        flags.is_on_ceiling |= temp.is_on_ceiling;
        flags.is_on_floor |= temp.is_on_floor;
        flags.is_on_wall |= temp.is_on_wall;
    }

    for(int i=0; i<sizeof(platforms_jump)/sizeof(Rectangle); i++){        
        const struct CollisionFlags temp_supersalto = collision_player_rectangle_simulate(player_current, platforms_jump[i]);
        flags.is_on_wall |= temp_supersalto.is_on_wall;
        flags.is_on_ceiling |= temp.is_on_ceiling;

        if(temp_supersalto.is_on_floor){
            player_current->velocity.y = -400;
            player_current->state = PLAYER_FALLING;
        }
    }

    return flags;
}

void level_test_draw(struct Player* player_current, Camera2D* camera, const float TIME_DELTA_STEP){
    const Rectangle platforms[] =  {
        (Rectangle){0, -100, 10, 100},
        (Rectangle){0, -200, 50, 40}
    };

    const Rectangle platforms_jump[] = {
        (Rectangle){100, -10, 50, 10},
        (Rectangle){150, -50, 50, 10}
    };

    // DIBUJO 
    BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Prueba", 20, 20, 40, GREEN);
        DrawText(TextFormat("Posicion: (%.2f, %.2f)", player_current->position.x, player_current->position.y), 10, 85, 20, BLACK);
        DrawText(TextFormat("Acceleracion: (%.2f, %.2f)", player_current->acceleration.x, player_current->acceleration.y), 10, 100, 20, BLACK);
    BeginMode2D(*camera);
    
        Color player_color = BLACK;
        switch(player_current->state){
            case PLAYER_FALLING:
                player_color = DARKGRAY;
                break;
            case PLAYER_IDDLE:
                player_color = BLUE;
            break;
            case PLAYER_WALKING:
                player_color = RED;
            break;
            case PLAYER_LANDING:
                player_color = GREEN;
            break;
        }
        DrawCircle(player_current->position.x, player_current->position.y, player_current->radius, player_color);

        DrawTexturePro(
            g_texture_jose_jose, 
            (Rectangle){0,0,g_texture_jose_jose.width, g_texture_jose_jose.height},
            (Rectangle){0,0,30,30},
            (Vector2){0,0},
            90, WHITE
        );

        for(int i=0; i<sizeof(platforms_jump)/sizeof(Rectangle); i++){
            DrawRectangleGradientH(platforms_jump[i].x, platforms_jump[i].y, platforms_jump[i].width, platforms_jump[i].height, WHITE, YELLOW);
            DrawRectangleLinesEx(platforms_jump[i], 1, BLACK);
        }

        DrawLineV((Vector2){-10000, 0}, (Vector2){10000, 0}, BLACK);
        DrawLineV((Vector2){0, -10000}, (Vector2){0, 10000}, BLACK);
        for(int i=0; i<sizeof(platforms)/sizeof(Rectangle); i++){
            DrawRectangleRec(platforms[i], BLACK);
        }
    EndMode2D();
    EndDrawing();
}