#ifndef UTILS_H
#define UTILS_H

#include "raylib.h"
#include <stdlib.h>
void DrawCubeTexture(Texture2D texture, Rectangle source, Vector3 position, float width, float height, float length, Color color); // Draw cube textured
void DrawCubeTopTexture(Texture2D texture, Rectangle source, Vector3 position, float width, float height, float length, Color color); // Draw cube textured
void DrawCubeBottomTexture(Texture2D texture, Rectangle source, Vector3 position, float width, float height, float length, Color color); // Draw cube textured
void DrawCubeLeftTexture(Texture2D texture, Rectangle source, Vector3 position, float width, float height, float length, Color color); // Draw cube textured
void DrawCubeRightTexture(Texture2D texture, Rectangle source, Vector3 position, float width, float height, float length, Color color); // Draw cube textured
void DrawCubeFrontTexture(Texture2D texture, Rectangle source, Vector3 position, float width, float height, float length, Color color); // Draw cube textured
void DrawCubeBackTexture(Texture2D texture, Rectangle source, Vector3 position, float width, float height, float length, Color color); // Draw cube textured

float rand_float(float min, float max);
double rand_double(double min, double max);

#endif