#include "sampling.h"

int main()
{
	pattle_t* pattle = genPattle();
	bindPattleColor(pattle, 0xE9ECEC, "minecraft:white_wool");
	bindPattleColor(pattle, 0xF07613, "minecraft:orange_wool");
	bindPattleColor(pattle, 0xBD44B3, "minecraft:magenta_wool");
	bindPattleColor(pattle, 0x3AAFD9, "minecraft:light_blue_wool");
	bindPattleColor(pattle, 0xF8C627, "minecraft:yellow_wool");
	bindPattleColor(pattle, 0x70B919, "minecraft:lime_wool");
	bindPattleColor(pattle, 0xED8DAC, "minecraft:pink_wool");
	bindPattleColor(pattle, 0x3E4447, "minecraft:gray_wool");
	bindPattleColor(pattle, 0x8E8E86, "minecraft:light_gray_wool");
	bindPattleColor(pattle, 0x158991, "minecraft:cyan_wool");
	bindPattleColor(pattle, 0x792AAC, "minecraft:purple_wool");
	bindPattleColor(pattle, 0x35399D, "minecraft:blue_wool");
	bindPattleColor(pattle, 0x724728, "minecraft:brown_wool");
	bindPattleColor(pattle, 0x546D1B, "minecraft:green_wool");
	bindPattleColor(pattle, 0xA12722, "minecraft:red_wool");
	bindPattleColor(pattle, 0x141519, "minecraft:black_wool");

	canvas_t* canvas = samplingFromImageSequence("G:\\Desktop\\1", pattle, 883, 64);

	convertCnavasToMCFunction(canvas, "test");
	genCommandChan(canvas, "test/chain.mcfunction");

	delCanvas(canvas);
}