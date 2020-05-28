#define _CRT_SECURE_NO_WARNINGS

#include "canvas.h"
#include <direct.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

char _buffer[2048];

// frame
typedef struct frame
{
	canvas_t* canvas;
	size_t** data;
} frame_t;

// pattle
typedef struct pattle
{
	char** block_ids;
	uint32_t* colors;
	size_t size;
} pattle_t;

// canvas
typedef struct canvas
{
	pattle_t* pattle;

	int width;
	int height;

	size_t frame_count;
	frame_t** frames;
} canvas_t;

canvas_t* genCanvas(int width, int height)
{
	canvas_t* canvas = malloc(sizeof(canvas_t));
	canvas->frame_count = 0;
	canvas->frames = 0;
	canvas->pattle = 0;
	canvas->width = width;
	canvas->height = height;
	return canvas;
}

void convertCnavasToMCFunction(canvas_t* canvas, const char* path)
{
	_mkdir(path);

	for (size_t i = 0; i < canvas->frame_count; ++i)
	{
		sprintf(_buffer, "%s/frame-%d.mcfunction", path, i);
		convertFrameToMCFunction(canvas->frames[i], _buffer);
	}
}

void setCanvasPattle(canvas_t* canvas, pattle_t* pattle)
{
	canvas->pattle = pattle;
}

pattle_t* getCanvasPattle(canvas_t* canvas)
{
	return canvas->pattle;
}

pattle_t* getFramePattle(frame_t* frame)
{
	return getCanvasPattle(frame->canvas);
}

pattle_t* genPattle()
{
	pattle_t* pattle = malloc(sizeof(pattle_t));
	pattle->block_ids = 0;
	pattle->colors = 0;
	pattle->size = 0;
	return pattle;
}

void delPattle(pattle_t* pattle)
{
	for (size_t i = 0; i < pattle->size; ++i)
		free(pattle->block_ids[i]);
	free(pattle->block_ids);
	free(pattle->colors);
	free(pattle);
}

int colorDifferent(uint32_t a, uint32_t b)
{
	uint8_t ar = (a << 8) >> 24;
	uint8_t ag = (a << 16) >> 24;
	uint8_t ab = (a << 24) >> 24;

	uint8_t br = (b << 8) >> 24;
	uint8_t bg = (b << 16) >> 24;
	uint8_t bb = (b << 24) >> 24;

	return abs((int)ar - (int)br) + abs((int)ag - (int)bg) + abs((int)ab - (int)bb);
}

size_t samplePattle(pattle_t* pattle, uint32_t color)
{
	int color_diff = 1000000;
	size_t id = -1;

	for (size_t i = 0; i < pattle->size; ++i)
	{
		int diff = colorDifferent(pattle->colors[i], color);
		if (
			id == -1 ||
			diff < color_diff
			)
		{
			color_diff = diff;
			id = i;
		}
	}
	return id;
}

const char* getPattleBlockId(pattle_t* pattle, uint32_t index)
{
	if (pattle >= 0 && index < pattle->size)
		return pattle->block_ids[index];
	return "";
}

void bindPattleColor(pattle_t* pattle, int color, const char* block_id)
{
	pattle->size++;
	if (pattle->block_ids == 0)
		pattle->block_ids = malloc(pattle->size * sizeof(const char*));
	else
	{
		void* _ptr = realloc(pattle->block_ids, sizeof(const char*) * pattle->size);
		if (_ptr != 0)
			pattle->block_ids = _ptr;
	}
	if (pattle->colors == 0)
		pattle->colors = malloc(pattle->size * sizeof(uint32_t));
	else
	{
		void* _ptr = realloc(pattle->colors, sizeof(uint32_t) * pattle->size);
		if (_ptr != 0)
			pattle->colors = _ptr;
	}
	char* _str = malloc((strlen(block_id) + 1) * sizeof(char));
	strcpy(_str, block_id);
	pattle->block_ids[pattle->size - 1] = _str;
	pattle->colors[pattle->size - 1] = color;
}

frame_t* genFrame(canvas_t* canvas)
{
	frame_t* frame = malloc(sizeof(frame_t));
	frame->canvas = canvas;
	frame->data = malloc(canvas->width * sizeof(size_t*));
	for (int i = 0; i < canvas->width; ++i)
		frame->data[i] = malloc(canvas->height * sizeof(size_t));
	return frame;
}

frame_t* genFrameAndAddToCanvas(canvas_t* canvas)
{
	frame_t* frame = genFrame(canvas);
	addFrame(canvas, frame);
	return frame;
}

void setFrameData(frame_t* frame, int x, int y, int pattle_id)
{
	if (x >= frame->canvas->width || y >= frame->canvas->height ||
		x < 0 || y < 0)
		return;
	frame->data[x][y] = pattle_id;
}

void addFrame(canvas_t* canvas, frame_t* frame)
{
	canvas->frame_count += 1;
	if (canvas->frames == 0)
		canvas->frames = malloc(canvas->frame_count * sizeof(frame_t*));
	else
	{
		void* _ptr = realloc(canvas->frames, sizeof(frame_t*) * canvas->frame_count);
		if (_ptr != 0)
			canvas->frames = _ptr;
	}
	canvas->frames[canvas->frame_count - 1] = frame;
}

void convertFrameToMCFunction(frame_t* frame, const char* path)
{
	FILE* file = fopen(path, "w");

	canvas_t* canvas = frame->canvas;
	pattle_t* pattle = canvas->pattle;

	for (int x = 0; x < canvas->width; ++x)
	{
		for (int y = 0; y < canvas->height; ++y)
		{
			const char* blockId = getPattleBlockId(pattle, frame->data[x][y]);
			fprintf(file, "setblock ~%d ~%d ~%d %s\n", x - canvas->width / 2, y - canvas->height / 2, 50, blockId);
		}
	}
	fclose(file);
}

void delCanvas(canvas_t* canvas)
{
	if (canvas->pattle != 0)
		delPattle(canvas->pattle);
	for (size_t i = 0; i < canvas->frame_count; ++i)
		delFrame(canvas->frames[i]);
	free(canvas->frames);
	free(canvas);
}

void delFrame(frame_t* frame)
{
	canvas_t* canvas = frame->canvas;
	for (int i = 0; i < canvas->width; ++i)
		free(frame->data[i]);
	free(frame->data);
	free(frame);
}

void genCommandChan(canvas_t* canvas, const char* path)
{
	FILE* file = fopen(path, "w");

	fprintf(file, "setblock ~-1 ~2 ~ minecraft:command_block[facing=down]{Command:\"tp @p ~ 100 ~\"}\n");
	fprintf(file, "setblock ~-1 ~1 ~ minecraft:chain_command_block[facing=down]{Command:\"say Begin!\",auto:1}\n");
	fprintf(file, "setblock ~-1 ~0 ~ minecraft:chain_command_block[facing=down]{Command:\"setblock ~1 ~-1 ~ minecraft:redstone_block\",auto:1}\n");
	
	int x = 0;
	int z = 0;

	for (size_t i = 0; i < canvas->frame_count; ++i)
	{
		int nextX = x + 1;
		int nextZ = z;
		if (nextX > 100)
		{
			nextX = 0;
			++nextZ;
		}
		int deltaX = nextX - x;
		int deltaZ = nextZ - z;

		fprintf(file, "setblock ~%d ~ ~%d minecraft:command_block[facing=up]{Command:\"setblock ~%d ~-1 ~%d minecraft:redstone_block\"}\n", x, z, deltaX, deltaZ, i);
		fprintf(file, "setblock ~%d ~1 ~%d minecraft:chain_command_block[facing=up]{Command:\"execute at @p run function custom:anim/frame-%d\", auto:1}\n", x, z, i);
		fprintf(file, "setblock ~%d ~2 ~%d minecraft:chain_command_block[facing=up]{Command:\"setblock ~ ~-3 ~ minecraft:air\", auto:1}\n", x, z, i);

		z = nextZ;
		x = nextX;
	}

	fclose(file);
}