#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION
#include "sampling.h"
#include <stb_image.h>

char _buffer[2048];

void samplingFrameFromRawData(frame_t* frame, unsigned char* data, int width, int height, int n, float scale_factor)
{
	pattle_t* pattle = getFramePattle(frame);

	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x)
		{
			int color = 0;
			for (int i = 0; i < n; ++i)
				color = data[(y * width + x) * n + i] + (color << 8);

			size_t id = samplePattle(pattle, color);
			setFrameData(frame, (int)((width - x - 1) * scale_factor), (int)((height - y - 1) * scale_factor), id);
		}
}

canvas_t* samplingFromImage(const char* path, pattle_t* pattle, int scaled_height)
{
	int width, height, n;
	unsigned char* image_data = stbi_load(path, &width, &height, &n, 0);
	float scale_factor = (float)scaled_height / height;

	canvas_t* canvas = genCanvas((int)(width * scale_factor), (int)(height * scale_factor));
	setCanvasPattle(canvas, pattle);
	frame_t* frame = genFrameAndAddToCanvas(canvas);

	samplingFrameFromRawData(frame, image_data, width, height, n, scale_factor);

	stbi_image_free(image_data);
	return canvas;
}

canvas_t* samplingFromImageSequence(const char* path, pattle_t* pattle, int count, int scaled_height)
{
	int width, height, n;
	sprintf(_buffer, "%s/%05d.png", path, 1);
	unsigned char* image_data = stbi_load(_buffer, &width, &height, &n, 0);
	float scale_factor = (float)scaled_height / height;
	canvas_t* canvas = genCanvas((int)(width * scale_factor), (int)(height * scale_factor));
	setCanvasPattle(canvas, pattle);
	stbi_image_free(image_data);

	for (int i = 1; i <= count; ++i)
	{
		sprintf(_buffer, "%s/%05d.png", path, i);
		unsigned char* image_data = stbi_load(_buffer, &width, &height, &n, 0);
		float scale_factor = (float)scaled_height / height;
		frame_t* frame = genFrameAndAddToCanvas(canvas);

		samplingFrameFromRawData(frame, image_data, width, height, n, scale_factor);

		stbi_image_free(image_data);

		printf("Loading %d/%d\n", i, count);
	}
	return canvas;
}