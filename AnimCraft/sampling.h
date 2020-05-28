#ifndef SAMPLING
#define SAMPLING

#include "canvas.h"

canvas_t* samplingFromImage(const char* path, pattle_t* pattle, int scaled_height);
canvas_t* samplingFromImageSequence(const char* path, pattle_t* pattle, int count, int scaled_height);

#endif