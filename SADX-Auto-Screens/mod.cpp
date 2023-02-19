#include "pch.h"
#include <direct.h>
#include <time.h>

HelperFunctions HelperFunctionsGlobal;
std::string modpath;
std::string modName = "Auto Screenshots";
time_t t;

extern "C" {
	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
		if (helperFunctions.Version < 14)
		{
			MessageBox(WindowHandle,
				L"Error, your version of the mod loader does not support API version 14. Some functionality will not be available.\nPlease exit the game and update the mod loader for the best experience.",
				L"Auto Screenshots Error: Mod Loader out of date", MB_OK | MB_ICONERROR);
		}


		HelperFunctionsGlobal = helperFunctions; // Save the helper pointer for external use

		modpath = path;
		ReadConfig(path, helperFunctions);
		srand((unsigned)time(&t));
		initScreenshots();
		std::string screenFolder = path;
		screenFolder += "\\screenshots";

		if (!IsPathExist(screenFolder)) //create screenshots folder if it doesn't exist
		{
			_mkdir(screenFolder.c_str());
		}

	}


	__declspec(dllexport) void __cdecl OnFrame()
	{

	}

	__declspec(dllexport) ModInfo SADXModInfo = { ModLoaderVer };
}