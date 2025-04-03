#define FNL_IMPL
#include "core.h"
#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include <math.h>
#include "raymath.h"
#include "rlgl.h" 

RenderTexture2D* textures;
Chunk* world[WORLD_SIZE][WORLD_SIZE];
Texture2D atlas;

#include "raylib.h"

void load_texture()
{
    atlas = LoadTexture("resources/textures/atlas.png");
}

Plane NormalizePlane(float a, float b, float c, float d) {
    float length = sqrtf(a * a + b * b + c * c);
    return (Plane) { (Vector3){ a / length, b / length, c / length }, d / length };
}

bool IsBoxInFrustum(Plane planes[6], BoundingBox box) {
    for (int i = 0; i < 6; i++) {
        Vector3 p = {
            (planes[i].normal.x > 0) ? box.max.x : box.min.x,
            (planes[i].normal.y > 0) ? box.max.y : box.min.y,
            (planes[i].normal.z > 0) ? box.max.z : box.min.z
        };
        if ((planes[i].normal.x * p.x + planes[i].normal.y * p.y + planes[i].normal.z * p.z + planes[i].d) < 0) {
            return false;
        }
    }
    return true;
}

void ExtractFrustumPlanes(Matrix vp, Plane planes[6]) {
    // Left Plane
    planes[0] = NormalizePlane(vp.m3  + vp.m0, vp.m7  + vp.m4, vp.m11 + vp.m8, vp.m15 + vp.m12);
    // Right Plane
    planes[1] = NormalizePlane(vp.m3  - vp.m0, vp.m7  - vp.m4, vp.m11 - vp.m8, vp.m15 - vp.m12);
    // Bottom Plane
    planes[2] = NormalizePlane(vp.m3  + vp.m1, vp.m7  + vp.m5, vp.m11 + vp.m9, vp.m15 + vp.m13);
    // Top Plane
    planes[3] = NormalizePlane(vp.m3  - vp.m1, vp.m7  - vp.m5, vp.m11 - vp.m9, vp.m15 - vp.m13);
    // Near Plane
    planes[4] = NormalizePlane(vp.m3  + vp.m2, vp.m7  + vp.m6, vp.m11 + vp.m10, vp.m15 + vp.m14);
    // Far Plane
    planes[5] = NormalizePlane(vp.m3  - vp.m2, vp.m7  - vp.m6, vp.m11 - vp.m10, vp.m15 - vp.m14);
}

void create_world()
{
    for(int chunkX = 0; chunkX < WORLD_SIZE; chunkX++)
    {
        for(int chunkZ = 0; chunkZ < WORLD_SIZE; chunkZ++)
        {
            world[chunkX][chunkZ] = malloc(sizeof(Chunk));
            generateChunk(world[chunkX][chunkZ], chunkX, chunkZ);
        }
    }
}
void draw_world(Camera camera)
{
    Plane frustum[6];
    Matrix view = GetCameraMatrix(camera);
    Matrix proj = MatrixPerspective(camera.fovy * DEG2RAD, 1280.0f / 720.0f, 0.1f, 1000.0f);
    Matrix vp = MatrixMultiply(view, proj);
    ExtractFrustumPlanes(vp, frustum);
    for (int chunkX = 0; chunkX < WORLD_SIZE; chunkX++)
    {
        for (int chunkZ = 0; chunkZ < WORLD_SIZE; chunkZ++)
        {   
            BoundingBox chunkBox = world[chunkX][chunkZ]->bounding_box;
            if(IsBoxInFrustum(frustum, chunkBox))
                draw_chunk(world[chunkX][chunkZ]);
        }
    }
}



void generateChunk(Chunk* chunk, int chunkX, int chunkZ) 
{
    fnl_state noise = fnlCreateState();
    noise.noise_type = FNL_NOISE_PERLIN;
    noise.seed = 318;
    noise.frequency = 0.014f + rand_float(0.01f, 0.03f);
    noise.fractal_type = FNL_FRACTAL_PINGPONG;
    noise.octaves = 8;
    noise.lacunarity = 2.450f;
    noise.gain = 0.590f;
    noise.weighted_strength = 1.040f;
    noise.ping_pong_strength = 1.290f;
    int chunkWorldX = chunkX * CHUNK_SIZE;
    int chunkWorldZ = chunkZ * CHUNK_SIZE;


    chunk->blocks = (Block*)malloc((CHUNK_SIZE*CHUNK_SIZE*CHUNK_HEIGHT) * sizeof(Block));
    if (!chunk->blocks) {
        printf("Failed to allocate memory for chunk blocks\n");
        free(chunk);
        return;
    }
    float minX = chunkX * CHUNK_SIZE;
    float minY = 0.0f;
    float minZ = chunkZ * CHUNK_SIZE;
    float maxX = minX + CHUNK_SIZE;
    float maxY = CHUNK_HEIGHT;
    float maxZ = minZ + CHUNK_SIZE;
    chunk->bounding_box = (BoundingBox){(Vector3){minX, minY, minZ}, (Vector3){maxX, maxY, maxZ}};

    for (int i = 0; i < CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT; i++) {
        int x = i % CHUNK_SIZE;  
        int y = (i / (CHUNK_SIZE * CHUNK_SIZE));
        int z = (i / CHUNK_SIZE) % CHUNK_SIZE;  
            int worldX = chunkX * CHUNK_SIZE + x;
            int worldZ = chunkZ * CHUNK_SIZE + z;

            float height = get_noise_at(&noise, worldX, worldZ);

            height = (height + 1.0f) * 6.0f;
            int blockHeight = (int)fmax(1.0f, fmin(height, CHUNK_HEIGHT - 1));
            chunk->blocks[i].pos = (Vector3){
            (float)x + chunkWorldX,
            (float)y,
            (float)z + chunkWorldZ 
            };
                if (y < blockHeight - 3) {
                    chunk->blocks[i].type = BEE_HIVE_SIDES_EMPTY;
                } else if (y < blockHeight - 1) {
                    chunk->blocks[i].type = WOOL_PINK;
                } else if (y == blockHeight - 1) {
                    chunk->blocks[i].type = BEET_1;
                } else {
                    chunk->blocks[i].type = AIR;
                }
    }
}
int validate_index(int index, int max, Chunk* chunk)
{
    if ((index < 0) || (index >= max))
        return 0;
    return chunk->blocks[index].type != AIR;
}

void validate_indexes(int left, int right, int top, int bottom, int front, int back, int max, int* canrender, Chunk* chunk)
{
    if (validate_index(left, max, chunk) &&
        validate_index(right, max, chunk) &&
        validate_index(top, max, chunk) &&
        validate_index(bottom, max, chunk) &&
        validate_index(front, max, chunk) &&
        validate_index(back, max, chunk)) {
        *canrender = 0;
    } else {
        *canrender = 1;
    }
}



void draw_chunk(Chunk* chunk)
{
    rlSetTexture(atlas.id);  // Set texture ONCE
    rlBegin(RL_QUADS);    
    int max = CHUNK_SIZE*CHUNK_SIZE*CHUNK_HEIGHT;
    for (int i = 0; i < CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT; i++) {
        int x = i % CHUNK_SIZE;  
        int y = (i / (CHUNK_SIZE * CHUNK_SIZE));
        int z = (i / CHUNK_SIZE) % CHUNK_SIZE; 
                Block block = chunk->blocks[i];
                 if(block.type != AIR)
                {
                    int can_render = 1;
                    int left   = (x > 0) ? i - 1 : -1;
                    int right  = (x < CHUNK_SIZE - 1) ? i + 1 : -1;
                    int bottom = (y > 0) ? i - (CHUNK_SIZE * CHUNK_SIZE) : -1;
                    int top    = (y < CHUNK_HEIGHT - 1) ? i + (CHUNK_SIZE * CHUNK_SIZE) : -1;
                    int front  = (z > 0) ? i - CHUNK_SIZE : -1;
                    int back   = (z < CHUNK_SIZE - 1) ? i + CHUNK_SIZE : -1;


                    validate_indexes(left, right, bottom, top, front, back, max, &can_render, chunk);

                    if (can_render == 1) 
                    {
                        Cell cell = get_cell_from_type(block.type);
                        Rectangle source = {
                            cell.column * CELL_WIDTH,
                            cell.row * CELL_HEIGHT,
                            CELL_WIDTH,
                            CELL_HEIGHT
                        };
                        DrawCubeTexture(atlas, source, block.pos, 1.0f, 1.0f, 1.0f, WHITE);
                    }

                }
    }
    rlEnd();                   // End batch AFTER all cubes
    rlSetTexture(0);   
}




Cell get_cell_from_type(int type)
{
    Cell cell;
    cell.column = type % COLUMNS; 
    cell.row = (type / COLUMNS) % ROWS; 
    return cell;
}

float get_noise_at(fnl_state* noise, int worldX, int worldZ)
{
    float n00 = fnlGetNoise2D(noise, worldX, worldZ);
    float n10 = fnlGetNoise2D(noise, worldX + 1, worldZ);
    float n01 = fnlGetNoise2D(noise, worldX, worldZ + 1);
    float n11 = fnlGetNoise2D(noise, worldX + 1, worldZ + 1);

    float blendX = worldX - (int)worldX;
    float blendZ = worldZ - (int)worldZ;

    float interpolatedX1 = n00 * (1 - blendX) + n10 * blendX;
    float interpolatedX2 = n01 * (1 - blendX) + n11 * blendX;

    return interpolatedX1 * (1 - blendZ) + interpolatedX2 * blendZ;
}