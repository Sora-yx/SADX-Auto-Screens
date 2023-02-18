#pragma once

static const uint8_t max = 20;
extern uint8_t character;
extern bool useSS;

void ReadConfig(const char* path, const HelperFunctions& helperFunctions);
extern uint8_t screenMax;
extern bool message;
extern bool sound;
extern std::string modName;
extern HelperFunctions HelperFunctionsGlobal;
extern std::string modpath;
void initScreenshots();
void ProcessQuickBoot();