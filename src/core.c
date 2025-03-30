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
    int index = 0;
    for (int z = 0; z < CHUNK_SIZE; z++) {
        for (int x = 0; x < CHUNK_SIZE; x++) {
            for (int y = 0; y < height; y++) {
                chunk->blocks[index].type = GRASS;
                chunk->blocks[index].pos = (Vector3){(float)x + pos.x, (float)y + pos.y, (float)z + pos.z};
                index++;
            }
        }
    }
    
    chunk->height = height;
    return chunk;
}

void generateChunk(Chunk* chunk, int chunkX, int chunkZ) {
    int index = 0;
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            int worldX = chunkX * CHUNK_SIZE + x;
            int worldZ = chunkZ * CHUNK_SIZE + z;
            float height = getHeight(worldX, worldZ);

            height = fmin(height, chunk->height - 1);

            for (int y = 0; y < chunk->height; y++) {
                if (y < height - 3) {
                    chunk->blocks[index].type = STONE;
                } else if (y < height) {
                    chunk->blocks[index].type = DIRT;
                } else if (y >= height && y < height + 1) {
                    chunk->blocks[index].type = GRASS;
                } else {
                    chunk->blocks[index].type = AIR;
                }
                index++;
            }
        }
    }
}

int validate_index(int index, int max, Chunk* chunk)
{
     if(index >= 0 && index < max){
        if (chunk->blocks[index].type == AIR) {
            return 0;
        }
        else
            return 1;
    }
    else
        return 0;

    return 0;
}
void validate_indexes(int left, int right, int top, int bottom, int front, int back, int max, int* canrender, Chunk* chunk)
{
    if(validate_index(left, max, chunk) != 0 &&
       validate_index(right, max, chunk) != 0 &&
       validate_index(top, max, chunk) != 0 &&
       validate_index(bottom, max, chunk) != 0 &&
       validate_index(front, max, chunk) != 0 &&
       validate_index(back, max, chunk) != 0) {
        *canrender = 0;
    }
}


void draw_chunk(Chunk* chunk)
{
    int index = 0;
    int max = CHUNK_SIZE*CHUNK_SIZE*chunk->height;
    for (int z = 0; z < CHUNK_SIZE; z++) {
        for (int x = 0; x < CHUNK_SIZE; x++) {
            for (int y = 0; y < chunk->height; y++) {
                Block block = chunk->blocks[index];
                if(block.type != AIR)
                {
                    int can_render = 1;

                    int left   = index - chunk->height;                      // x - 1
                    int right  = index + chunk->height;                      // x + 1
                    int bottom = index - 1;                                  // y - 1
                    int top    = index + 1;                                  // y + 1
                    int front  = index - CHUNK_SIZE * chunk->height;         // z - 1
                    int back   = index + CHUNK_SIZE * chunk->height;         // z + 1

                    validate_indexes(left, right, bottom, top, front, back, max, &can_render, chunk);

                    if (can_render == 1) 
                    {
                        RenderTexture2D* tex = get_texture_from_type(block.type);
                        DrawCubeTexture(tex->texture, block.pos, 1.0f, 1.0f, 1.0f, WHITE);
                        DrawCubeWires(block.pos, 1.0f, 1.0f, 1.0f, DARKGRAY);
                    }
                }
                index++;
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
    switch(type)
    {
        case AIR:
            cell.row = 0;
            cell.column = 0;
            break;
        case GRASS:
            cell.row = 3;
            cell.column = 29;
            break;
        case STONE:
            cell.row = 3;
            cell.column = 22;
            break;
        case DIRT:
            cell.row = 0;
            cell.column = 26;
            break;
        case LOG:
            cell.row = 0;
            cell.column = 0;
            break;
        case LEAVES:
            cell.row = 0;
            cell.column = 0;
            break;
        case GLASS:
            cell.row = 0;
            cell.column = 0;
            break;
        case SAND:
            cell.row = 0;
            cell.column = 0;
            break;
        case WATER:
            cell.row = 0;
            cell.column = 0;
            break;
        case PLANKS:
            cell.row = 0;
            cell.column = 0;
            break;
        case CRAFTING_TABLE:
            cell.row = 0;
            cell.column = 0;
            break;
        default:
            cell.row = 0;
            cell.column = 0;
            break;

    }
    return cell;
}