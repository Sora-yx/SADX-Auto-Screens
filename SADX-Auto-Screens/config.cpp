#include "pch.h"

bool message = true;
bool sound = true;
uint8_t screenMax = 12;
uint8_t character = Characters_Sonic;
bool useSS = false;

void ReadConfig(const char* path, const HelperFunctions& helperFunctions) {
	//Ini file Configuration
	const IniFile* config = new IniFile(std::string(path) + "\\config.ini");
	screenMax = (uint8_t)config->getInt("main", "screen", screenMax);
	message = config->getBool("main", "msg", message);
	sound = config->getBool("main", "sound", sound);
	character = (uint8_t)config->getInt("main", "char", character);

	delete config;

	if (screenMax > max)
		screenMax = max;
	else if (screenMax < 2)
		screenMax = 2;

	if (character == Characters_MetalSonic)
	{
		character = Characters_Sonic;
		MetalSonicFlag = 1;
	}	

	if (character == 9)
	{
		useSS = true;
		character = Characters_Sonic;
	}
}