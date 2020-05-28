#ifndef CANVAS
#define CANVAS

#include <stdint.h>

typedef struct frame frame_t;
typedef struct canvas canvas_t;
typedef struct pattle pattle_t;

canvas_t* genCanvas(int width, int height);
frame_t* genFrame(canvas_t* canvas);
pattle_t* genPattle();

frame_t* genFrameAndAddToCanvas(canvas_t* canvas);
void addFrame(canvas_t* canvas, frame_t* frame);
void setCanvasPattle(canvas_t* canvas, pattle_t* pattle);
void convertCnavasToMCFunction(canvas_t* frame, const char* path);
pattle_t* getCanvasPattle(canvas_t* canvas);
void genCommandChan(canvas_t* canvas, const char* path);

void setFrameData(frame_t* frame, int x, int y, int pattle_id);
void convertFrameToMCFunction(frame_t* frame, const char* path);
pattle_t* getFramePattle(frame_t* frame);

void bindPattleColor(pattle_t* pattle, int color, const char* block_id);
size_t samplePattle(pattle_t* pattle, uint32_t color);
const char* getPattleBlockId(pattle_t* pattle, uint32_t index);

void delCanvas(canvas_t* canvas);
void delFrame(frame_t* frame);
void delPattle(pattle_t* pattle);

#endif