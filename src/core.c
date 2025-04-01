#include "core.h"
#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include <math.h>
RenderTexture2D* textures;
void free_textures_array()
{
    free(textures);
}
void init_textures_array()
{
    textures = (RenderTexture2D*)malloc(TEXTURES_COUNT * sizeof(RenderTexture2D));
    for(int i = 0; i < TEXTURES_COUNT;  i++)
    {
        Cell cell = get_cell_from_type(i);
        textures[i] = get_texture_from_atlas(cell);
    }
}

RenderTexture2D* get_texture_from_type(int type)
{
    return &textures[type];
}

Chunk* create_chunk(int height, Vector3 pos){
    Chunk* chunk = (Chunk*)malloc(sizeof(Chunk));
    if (!chunk) {
        printf("Failed to allocate memory for chunk\n");
        return NULL;
    }

    chunk->blocks = (Block*)malloc((CHUNK_SIZE*CHUNK_SIZE*height) * sizeof(Block));
    if (!chunk->blocks) {
        printf("Failed to allocate memory for chunk blocks\n");
        free(chunk);
        return NULL;
    }
 for (int i = 0; i < CHUNK_SIZE * CHUNK_SIZE * height; i++) {
        int x = i % CHUNK_SIZE;  
        int y = (i / (CHUNK_SIZE * CHUNK_SIZE));
        int z = (i / CHUNK_SIZE) % CHUNK_SIZE;  

        chunk->blocks[i].type = PORTAL;
        chunk->blocks[i].pos = (Vector3){
            (float)x + pos.x,
            (float)y + pos.y,
            (float)z + pos.z 
        };
    }
    
    chunk->height = height;
    return chunk;
}

void generateChunk(Chunk* chunk, int chunkX, int chunkZ) {
    int index = 0;
    float* noise_data = get_noise();

    for (int i = 0; i < CHUNK_SIZE * CHUNK_SIZE * chunk->height; i++) {
        int x = i % CHUNK_SIZE;  
        int y = (i / (CHUNK_SIZE * CHUNK_SIZE));
        int z = (i / CHUNK_SIZE) % CHUNK_SIZE;  
            int worldX = chunkX * CHUNK_SIZE + x;
            int worldZ = chunkZ * CHUNK_SIZE + z;

            int noiseIndex = (worldZ % 128) * 128 + (worldX % 128);
            float height = noise_data[noiseIndex];

            height = (height + 1.0f) * 16.0f;
            int blockHeight = (int)fmax(1.0f, fmin(height, chunk->height - 1));

                if (y < blockHeight - 3) {
                    chunk->blocks[index].type = OBSIDIAN;
                } else if (y < blockHeight - 1) {
                    chunk->blocks[index].type = OBSIDIAN;
                } else if (y == blockHeight - 1) {
                    chunk->blocks[index].type = OBSIDIAN;
                } else {
                    chunk->blocks[index].type = OBSIDIAN;
                }
                index++;
    }

    free(noise_data);
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
    int max = CHUNK_SIZE*CHUNK_SIZE*chunk->height;
    for (int i = 0; i < CHUNK_SIZE * CHUNK_SIZE * chunk->height; i++) {
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
                    int top    = (y < chunk->height - 1) ? i + (CHUNK_SIZE * CHUNK_SIZE) : -1;
                    int front  = (z > 0) ? i - CHUNK_SIZE : -1;
                    int back   = (z < CHUNK_SIZE - 1) ? i + CHUNK_SIZE : -1;


                    validate_indexes(left, right, bottom, top, front, back, max, &can_render, chunk);

                    if (can_render == 1) 
                    {
                        RenderTexture2D* tex = get_texture_from_type(block.type);
                        DrawCubeTexture(tex->texture, block.pos, 1.0f, 1.0f, 1.0f, WHITE);
                        DrawCubeWires(block.pos, 1.0f, 1.0f, 1.0f, DARKGRAY);
                    }

                }
    }
}

RenderTexture2D get_texture_from_atlas(Cell cell)
{
    Texture2D atlas = LoadTexture("resources/atlas.png");
    Rectangle source = {
        cell.column * CELL_WIDTH,
        cell.row * CELL_HEIGHT,
        CELL_WIDTH,
        CELL_HEIGHT
    };
     RenderTexture2D croppedRender = LoadRenderTexture(CELL_WIDTH, CELL_HEIGHT);
     BeginTextureMode(croppedRender);
        ClearBackground(BLANK);
        DrawTextureRec(atlas, source, (Vector2){ 0, 0 }, WHITE);
    EndTextureMode();
    return croppedRender;
}

Cell get_cell_from_type(int type)
{
    Cell cell;
    int columns = 64;
    int rows = 32;
    
    cell.column = type % columns; 
    
    cell.row = (type / (columns * rows));
    printf("column: %d, row: %d\n", cell.column, cell.row);
    return cell;
}