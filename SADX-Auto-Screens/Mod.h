#pragma once


struct Photos
{
	uint8_t lvl;
	uint8_t act;
	NJS_POINT3 pos;
	int rotY;
	NJS_POINT3 camPos;
	Angle3 camAng;
	GameModes gameMode;
};


static const uint8_t max = 20;
extern uint8_t character;
extern bool useSS;

extern Photos destArray[];

void ReadConfig(const char* path, const HelperFunctions& helperFunctions);
extern uint8_t screenMax;
extern bool message;
extern bool sound;
extern std::string modName;
extern HelperFunctions HelperFunctionsGlobal;
extern std::string modpath;
void initScreenshots();
void ProcessQuickBoot();
void SetPatches();