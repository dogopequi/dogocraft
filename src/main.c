#include "core.h"
#include <stdio.h>
#include "rlgl.h" 

int main(void)
{
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "dogocraft");
    InitAudioDevice();
    Camera camera = { 0 };
    camera.position = (Vector3){ 0.0f, 2.0f, 4.0f };
    camera.target = (Vector3){ 0.0f, 2.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    int cameraMode = CAMERA_FIRST_PERSON;

    DisableCursor();
    SetTargetFPS(60);

    init_textures_array();
    Chunk* chunk2 = create_chunk(32, (Vector3){0.0f, 0.0f, 0.0f});
  /*   Chunk* chunk3 = create_chunk(32, (Vector3){-16.0f, 0.0f, 0.0f});
    Chunk* chunk4 = create_chunk(32, (Vector3){0.0f, 0.0f, 16.0f}); */
    generateChunk(chunk2, 2, 16);
/*      generateChunk(chunk3, 0, 24);
    generateChunk(chunk4, 5, 12);  */
    printf("hre");
    while (!WindowShouldClose())
    {
        UpdateCameraPro(&camera,
            (Vector3){
                (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))*0.1f -      // Move forward-backward
                (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))*0.1f,    
                (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))*0.1f -   // Move right-left
                (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))*0.1f,
                (IsKeyDown(KEY_SPACE))*0.1f -   // Move right-left
                (IsKeyDown(KEY_LEFT_CONTROL))*0.1f,                                            // Move up-down
            },
            (Vector3){
                GetMouseDelta().x*0.05f,                            // Rotation: yaw
                GetMouseDelta().y*0.05f,                            // Rotation: pitch
                0.0f                                                // Rotation: roll
            },
            GetMouseWheelMove()*2.0f);                              // Move to target (zoom)
        BeginDrawing();
        ClearBackground(RAYWHITE); 
        BeginMode3D(camera);
        draw_chunk(chunk2);
/*          draw_chunk(chunk3);
        draw_chunk(chunk4);  */
        EndMode3D();
        DrawFPS(10, 10);
        EndDrawing();
    }
    free_textures_array();
    CloseWindow(); 
    CloseAudioDevice();
    return 0;
    // TODO collision, multiple chunks, water
}

