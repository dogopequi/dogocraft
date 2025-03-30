#ifndef UTILS_H
#define UTILS_H

#include "raylib.h"
#include <stdlib.h>
void DrawCubeTexture(Texture2D texture, Vector3 position, float width, float height, float length, Color color); // Draw cube textured
void DrawCubeTopTexture(Texture2D texture, Vector3 position, float width, float height, float length, Color color); // Draw cube textured

float getHeight(int x, int z);

#endif