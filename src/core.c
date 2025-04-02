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
    atlas = LoadTexture("resources/atlas.png");
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
    for (int chunkX = 0; chunkX < WORLD_SIZE; chunkX++)
    {
        for (int chunkZ = 0; chunkZ < WORLD_SIZE; chunkZ++)
        {
/*             if (is_chunk_visible(camera, world[chunkX][chunkZ], chunkX, chunkZ))
            { */
                draw_chunk(world[chunkX][chunkZ]);
/*             } */
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

/*         if(chunk->blocks[i].type != AIR)
        {
            Cell cell = get_cell_from_type(chunk->blocks[i].type);
            //chunk->blocks[i].mesh = CreateCubeWithAtlas(&cell);
        } */
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