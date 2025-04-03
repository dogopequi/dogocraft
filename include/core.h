#ifndef CORE_H
#define CORE_H
#include "raylib.h"
#include "utils.h"
#include "FastNoiseLite.h"

typedef struct {
    Vector3 normal;
    float d;
} Plane;


#define CHUNK_SIZE 16
#define CHUNK_HEIGHT 32
#define WORLD_SIZE 3

#define CUBE_SIZE 32

#define COLUMNS 33
#define ROWS 34

#define CELL_WIDTH 16
#define CELL_HEIGHT 16
#define AIR -1
typedef enum {
    NOT_USED0,
    NOT_USED1,
    NOT_USED2,
    NOT_USED3,
    NOT_USED4,
    NOT_USED5,
    NOT_USED6,
    NOT_USED7,
    NOT_USED8,
    BOOK_0,
    BOOK_1,
    BOOK_2,
    NOT_USED9,
    NOT_USED10,
    NOT_USED11,
    BEE_HIVE_SIDES_EMPTY,
    BEET_0,
    BIRCH_PLANK,
    WOOL_DARK_BROWN,
    DEAD_VINE_0,
    DROPPER_FRONT,
    FIRE_0,
    NOT_USED12,
    NOT_USED13,
    NOT_USED14,
    NOT_USED15,
    LADDER,
    NOT_USED16,
    NOT_USED17,
    WOOL_PINK,
    MUSHROOM_NETHER,
    WOOL_ORANGE,
    NOT_USED18,
    WEIRD_PLANT_0,
    WOOL_PURPLE,
    NOT_USED19,
    NOT_USED20,
    NOT_USED21,
    NOT_USED22,
    NOT_USED23,
    NOT_USED24,
    NOT_USED25,
    NOT_USED26,
    NOT_USED27,
    NOT_USED28,
    NOT_USED29,
    NOT_USED30,
    NOT_USED31,
    NOT_USED32,
    NOT_USED33,
    BEE_HIVE_SIDES_FULL,
    BEET_1,
    SAPLING_TREE_IDK

} BlockType;



typedef struct Cell{
    int column;
    int row; 
} Cell;
typedef struct Block{
    int type;
    Vector3 pos;
    Mesh mesh;

} Block;

typedef struct Chunk {
    Block* blocks;
    BoundingBox bounding_box;
} Chunk;

void draw_chunk(Chunk* chunk);
int validate_index(int index, int max, Chunk* chunk);
void validate_indexes(int left, int right, int top, int bottom, int front, int back, int max, int* canrender, Chunk* chunk);
void generateChunk(Chunk* chunk, int chunkX, int chunkZ);
Cell get_cell_from_type(int type);
float get_noise_at(fnl_state* noise, int worldX, int worldZ);
void load_texture();
void create_world();
void draw_world(Camera camera);
extern RenderTexture2D* textures;
extern Chunk* world[WORLD_SIZE][WORLD_SIZE];
extern Texture2D atlas;

Plane NormalizePlane(float a, float b, float c, float d);
void ExtractFrustumPlanes(Matrix vp, Plane planes[6]);
bool IsBoxInFrustum(Plane planes[6], BoundingBox box);

#endif