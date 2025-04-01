#ifndef CORE_H
#define CORE_H
#include "raylib.h"
#include "utils.h"
#define CHUNK_SIZE 16
#define CUBE_SIZE 32

#define CELL_WIDTH 16
#define CELL_HEIGHT 16
#define TEXTURES_COUNT 47
#define AIR -1
#define LAVA_0 1
#define LAVA_1 2
#define WATER_0 3
#define WATER_1 4
#define WHITE_BED 5
#define BOOKSHELF 6
#define CACTUS_INNER 7
#define CAKE 8
#define IDK 9
#define DROPPER_FRONT 10
#define WHITE_GRASS 11
#define PLANT 13
#define OBSIDIAN 12
#define DIRT_HOED 14
#define WHITE_FLOWER 15
#define FURNACE 16
#define WOODEN_PLANKS 17
#define QUARTZ_INNER 18
#define REDSTONE_DUST 19
#define SAPLING 20
#define SOME_BLOCK 21
#define WATER_DROP 22
#define WOOL_GREEN 23
#define LAVA_2 24
#define LAVA_3 25
#define WATER_2 26
#define WATER_3 27
#define BED_RED 28
#define POTION 29
#define CACTUS_LOG 30
#define CAKE_OTHER 31
#define IDK1 32
#define DOOR_WOODEN_WHITE 33
#define PLANT_CHERRY 34
#define PLANT_MIDDLE 35
#define ENCHANTING_TABLE_SIDE 36
#define OTHER_SAPLING 37
#define SMALL_DOOR 38
#define FURNACE_SIDE 39
#define PORTAL 40
#define QUARTZ_SIDE 41
#define CANTSE 42
#define BIRCH_SAPLING 43
#define STONE_SLAB 44
#define LILYPAD 45
#define WOOL_BLUE 46



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
void init_textures_array();
void free_textures_array();
extern RenderTexture2D* textures;

#endif