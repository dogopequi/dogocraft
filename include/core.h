#ifndef CORE_H
#define CORE_H
#include "raylib.h"
#include "utils.h"
#include "fastnoiselite.h"
#define CHUNK_SIZE 16
#define WORLD_SIZE 5

#define CUBE_SIZE 32
#define COLUMNS 23
#define ROWS 16
#define CELL_WIDTH 16
#define CELL_HEIGHT 16
#define TEXTURES_COUNT 46
#define AIR -1
#define LAVA_0 0
#define LAVA_1 1
#define WATER_0 2
#define WATER_1 3
#define WHITE_BED 4
#define BOOKSHELF 5
#define CACTUS_INNER 6
#define CAKE_0 7
#define IDK 8
#define DROPPER_FRONT 9
#define WHITE_GRASS 10
#define PLANT_TOP 11
#define OBSIDIAN 12
#define DIRT_HOED 13
#define WHITE_FLOWER 14
#define FURNACE 15
#define WOODEN_PLANKS 16
#define QUARTZ_INNER 17
#define REDSTONE_DUST 18
#define SAPLING 19
#define SOME_BLOCK 20
#define WATER_DROP 21
#define WOOL_GREEN 22
#define LAVA_2 23
#define LAVA_3 24
#define WATER_2 25
#define WATER_3 26
#define BED_RED 27
#define POTION 28
#define CACTUS_LOG 29
#define CAKE_OTHER 30
#define IDK1 31
#define DOOR_WOODEN_WHITE 32
#define PLANT_CHERRY 33
#define PLANT_MIDDLE 34
#define ENCHANTING_TABLE_SIDE 35
#define OTHER_SAPLING 36
#define SMALL_DOOR 37
#define FURNACE_SIDE 38
#define PORTAL 39
#define QUARTZ_SIDE 40
#define CANTSE 41
#define BIRCH_SAPLING 42
#define STONE_SLAB 43
#define LILYPAD 44
#define WOOL_BLUE 45



typedef struct Cell{
    int column;
    int row; 
} Cell;
typedef struct Block{
    int type;
    Vector3 pos;

} Block;

typedef struct Chunk {
    Block* blocks;
    int height;

} Chunk;

Chunk* create_chunk(int height, Vector3 pos);
void draw_chunk(Chunk* chunk);
int validate_index(int index, int max, Chunk* chunk);
void validate_indexes(int left, int right, int top, int bottom, int front, int back, int max, int* canrender, Chunk* chunk);
void generateChunk(Chunk* chunk, int chunkX, int chunkZ);
Cell get_cell_from_type(int type);
RenderTexture2D get_texture_from_atlas(Cell cell);
float get_noise_at(fnl_state* noise, int worldX, int worldZ);
void init_textures_array();
void free_textures_array();
void create_world();
void draw_world();

extern RenderTexture2D* textures;
extern Chunk* world[WORLD_SIZE][WORLD_SIZE];

#endif