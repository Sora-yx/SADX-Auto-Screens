#include "pch.h"
#include "FunctionHook.h"
#include <atlimage.h>
#include <ShellApi.h>

static FunctionHook<void> LoadLevelObject_t(LoadLevelObject);
static FunctionHook<void, taskwk*> SetPlayerInitialPosition_t(SetPlayerInitialPosition);
static uint8_t count = 0;

static bool camHack = false;

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

static Photos screenArray[]
{
	{ LevelIDs_EmeraldCoast, 0, {-627, 116, 1186}, 16384, {-636, 123, 1196}, {64819, 58146, 0}, GameModes_Trial  },
	{ LevelIDs_EmeraldCoast, 0, {4298, 25, 375}, 19006, {0}, {0}, GameModes_Trial },
	{ LevelIDs_EmeraldCoast, 1, {3292, 70, -2711}, {2444}, {3305, 78.11, -2699}, {64531, 7966, 0}, GameModes_Trial  },
	{ LevelIDs_WindyValley, 2, {5801, -10744, -809}, {12548}, {5799.0f, -10747.0f, -793.08f}, {64360, 64497, 0}, GameModes_Trial  },
	{ LevelIDs_Casinopolis, 0,  {-373, 137, 297}, {0x8000}, {-382, 145, 308}, {63948, 58808, 0x0}, GameModes_Trial  }, //pirate ship
	{ LevelIDs_Casinopolis, 0,  {-289, -198, -582}, {12241}, {-283, -192, -568}, {65088, 3948, 0x0}, GameModes_Trial  }, //shower
	{ LevelIDs_IceCap, 0, {488, 249, 1118 }, {11790}, {491, 256, 1130}, {64652, 2932, 0}, GameModes_Trial },
	{ LevelIDs_TwinklePark, 1,  {755, 55, -314}, {44508}, {749, 62, -327}, {13, 37641, 0}, GameModes_Trial  },
	{ LevelIDs_SpeedHighway, 0,  {2282, -1311, 1967}, {43713}, {2272, -1304, 1959}, {64391, 43034, 0}, GameModes_Trial  },
	{ LevelIDs_SpeedHighway, 0,  {3235, -1500, 3139}, {45853}, {3228, -1494, 3131}, {65151, 41595, 0}, GameModes_Trial  },
	{ LevelIDs_SpeedHighway, 1,  {24, 28, -19}, {64320}, {0}, {0}, GameModes_Trial  },
	{ LevelIDs_SpeedHighway, 2,  {-263, 10, -1299}, {23533}, {-274, 15, -1289}, {822, 57731, 0}, GameModes_Trial },
	{ LevelIDs_RedMountain, 0,  {-3272, 758, -1630}, {34956}, {-3292, 765, -1636}, {29, 45430, 0}, GameModes_Trial  },
	{ LevelIDs_RedMountain, 0,  {-3762, 1121, -1945}, {4124}, {-3753, 1128, -1933}, {65251, 2961, 0}, GameModes_Trial  },
	{ LevelIDs_RedMountain, 1,  {-2797, 204, 1241}, {56997}, {-2791, 212, 1228}, {64965, 28702, 0}, GameModes_Trial  },
	{ LevelIDs_SkyDeck, 1,  {243, -83, 2461}, {23219}, {230, -77, 2471}, {150, 55354, 0}, GameModes_Trial },
	{ LevelIDs_LostWorld, 0,  {46, -72, 128}, {37477}, {36, -66, 115}, {430, 40624, 0}, GameModes_Trial  },
	{ LevelIDs_FinalEgg, 2,  {80, -3222, -734}, {22308}, {79, -3215, -710}, {65449, 65193, 0}, GameModes_Trial  },
	{ LevelIDs_MysticRuins, 1, {-0, -157, 2216}, {22294}, {-10, -149, 2228}, {64874, 59626, 0}, GameModes_Adventure_Field },
	{ LevelIDs_Past, 0, {1, 630, 20}, {62106}, {10, 638, 9}, {63716, 28650, 0}, GameModes_Adventure_Field },
	{ LevelIDs_Past, 1, {4, 7, 251}, {17196}, {4, 12, 273}, {65396, 65502, 0}, GameModes_Adventure_Field },
	{ LevelIDs_SSGarden, 0, {-43, 0, 14}, {32388}, {-60, 6, 14}, {65368, 49416, 0}, GameModes_Adventure_Field },
	{ LevelIDs_ECGarden, 0, {-83, 7, -233}, {65210}, {-71, 15, -226}, {64468, 10389, 0}, GameModes_Adventure_Field },
	{ LevelIDs_MRGarden, 0, {-61, 46, 10}, {21313}, {-75, 53, 24}, {64708, 56418, 0}, GameModes_Adventure_Field },
};

static Photos destArray[max] = { NULL };

std::vector<uint8_t> levelDuplicate;

void SetNewCamPos(NJS_POINT3 pos)
{
	Camera_Data1->Position = pos;
}

void SetNewCamAng(Angle3 pos)
{
	Camera_Data1->Rotation.x = pos.x;
	Camera_Data1->Rotation.y = pos.y;
	Camera_Data1->Rotation.z = pos.z;
}

void SetCamForScreenshot() {

	if (destArray[count].camPos.x == 0 && destArray[count].camPos.y == 0 && destArray[count].camPos.z == 0)
	{
		camHack = false;
		return;
	}


	SetNewCamPos(destArray[count].camPos);
	SetNewCamAng(destArray[count].camAng);
}

void CamOnFrames()
{
	if (camHack)
	{
		WriteData<1>((int*)0x437760, 0x75); //stop normal camera
		CameraType[0] = 0;
		CameraType[1] = 0;
		CameraType[2] = 0;
		WriteData((int*)0x3B2CAE3, 0x462E9002);
		WriteData((int*)0x3B2CAE7, 0x467D8000);
		WriteData<1>((int*)0x3B2CBA8, 4); // camera_flags
	}
	else
	{
		WriteData<1>((int*)0x437760, 0x74); // Enable normal cameras
		WriteData<1>((int*)0x436308, 0x8F); // Enable first person cam
	}
}


bool saveBitmap(LPCSTR filename, HBITMAP bmp, HPALETTE pal)
{
	bool result = false;
	PICTDESC pd;

	pd.cbSizeofstruct = sizeof(PICTDESC);
	pd.picType = PICTYPE_BITMAP;
	pd.bmp.hbitmap = bmp;
	pd.bmp.hpal = pal;

	LPPICTURE picture;
	HRESULT res = OleCreatePictureIndirect(&pd, IID_IPicture, false,
		reinterpret_cast<void**>(&picture));

	if (!SUCCEEDED(res))
		return false;

	LPSTREAM stream;
	res = CreateStreamOnHGlobal(0, true, &stream);

	if (!SUCCEEDED(res))
	{
		picture->Release();
		return false;
	}

	LONG bytes_streamed;
	res = picture->SaveAsFile(stream, true, &bytes_streamed);

	HANDLE file = CreateFileA(filename, GENERIC_WRITE, FILE_SHARE_READ, 0,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (!SUCCEEDED(res) || !file)
	{
		stream->Release();
		picture->Release();
		return false;
	}

	HGLOBAL mem = 0;
	GetHGlobalFromStream(stream, &mem);
	LPVOID data = GlobalLock(mem);

	DWORD bytes_written;

	result = !!WriteFile(file, data, bytes_streamed, &bytes_written, 0);
	result &= (bytes_written == static_cast<DWORD>(bytes_streamed));

	GlobalUnlock(mem);
	CloseHandle(file);

	stream->Release();
	picture->Release();

	return result;
}

bool screenCapturePart(int x, int y, int w, int h, LPCSTR fname)
{
	HDC hdcSource = GetDC(NULL);
	HDC hdcMemory = CreateCompatibleDC(hdcSource);

	int capX = GetDeviceCaps(hdcSource, HORZRES);
	int capY = GetDeviceCaps(hdcSource, VERTRES);

	HBITMAP hBitmap = CreateCompatibleBitmap(hdcSource, w, h);
	HBITMAP hBitmapOld = (HBITMAP)SelectObject(hdcMemory, hBitmap);

	BitBlt(hdcMemory, 0, 0, w, h, hdcSource, x, y, SRCCOPY);
	hBitmap = (HBITMAP)SelectObject(hdcMemory, hBitmapOld);

	DeleteDC(hdcSource);
	DeleteDC(hdcMemory);

	HPALETTE hpal = NULL;

	if (saveBitmap(fname, hBitmap, hpal))
		return true;

	return false;
}

void SetScreenShot(task* tp)
{
	if (camera_twp)
	{
		CamOnFrames();
		SetCamForScreenshot();
	}

	//if we aren't in game or if a cutscene is running, abort
	if (GameState != 15 || EV_MainThread_ptr)
		return;

	auto twp = tp->twp;
	std::string folder = modpath + "\\screenshots";
	std::string path = folder + "\\" + std::to_string(count) + ".jpg";
	int crop = IsWindowed ? 15 : 0;

	switch (twp->mode)
	{
	case 0:
		camHack = true;

		ForcePlayerAction(0, useSS ? 46 : 24);
		playerpwp[0]->item |= Powerups_Invincibility;

		GameMode = destArray[count].gameMode;
		DisablePause();
		DisableControl();
		twp->mode++;
		break;
	case 1:
		if (++twp->wtimer == 120) //wait one second
		{
			twp->wtimer = 0;
			twp->mode++;
		}
		break;
	case 2:
		if (screenCapturePart(0, crop, HorizontalResolution, VerticalResolution - crop, path.c_str())) //take a screenshot
		{
			if (sound)
			{
				PlayVoice(6875);
			}

			twp->mode++;
		}
		else
		{
			PrintDebug("Failed to generate screenshot\n");
			FreeTask(tp);
			return;
		}
		break;
	case 3:
		if (++twp->wtimer == 30)
		{
			count++;

			if (count < screenMax)
			{
				SetNextLevelAndAct_CutsceneMode(destArray[count].lvl, destArray[count].act);
				twp->mode++;
				FreeTask(tp);
				return;
			}

			if (message)
			{
				MessageBox(WindowHandle, L"All the screenshots have been successfully generated!", L"Auto Screens Mod", MB_OK | MB_ICONINFORMATION);
			}

			ShellExecuteA(GetDesktopWindow(), "open", folder.c_str(), NULL, NULL, SW_SHOW); //open screenshots folder
			Sleep(5);
			FreeTask(tp);
			exit(0);
		}

		break;
	}
}

void LoadLevelObject_r()
{
	LoadLevelObject_t.Original();
	CreateElementalTask(2, 2, SetScreenShot);
}

void GenerateRandomLevelsLocation()
{
	//fill the destination array
	for (uint8_t i = 0; i < screenMax; i++)
	{
		uint8_t id = (uint8_t)rand() % LengthOfArray(screenArray);
		uint8_t curLvl = screenArray[id].lvl;

		if (i < 13) //avoid duplicate if there is less than 12 screenshots requested
		{
			do //do another rand if the level generated is already stored in the destination array
			{
				id = (uint8_t)rand() % LengthOfArray(screenArray);
				curLvl = screenArray[id].lvl;

			} while (std::count(levelDuplicate.begin(), levelDuplicate.end(), curLvl));

			levelDuplicate.push_back(screenArray[id].lvl); //add level generated in a vector to check duplicate
		}

		destArray[i] = screenArray[id];

		if (!i)
		{
			CurrentLevel = destArray[i].lvl;
			CurrentAct = destArray[i].act;
		}
	}
}

void HideHud_r()
{
	WriteData<1>((int*)0x425F90, 0xC3);
	WriteData<1>((int*)0x0427F50, 0xC3);
	WriteData<1>((int*)0x0457120, 0xC3);
	WriteData<1>((int*)0x04B3830, 0xC3);
	WriteData<1>((int*)0x0628330, 0xC3);
	WriteData<1>((int*)0x04DB5E0, 0xC3);
	WriteData<1>((int*)0x0470546, 0xEB);
	WriteData<1>((int*)0x04706D6, 0xEB);
	WriteData<1>((int*)0x046FB00, 0xC3);
	WriteData<1>((int*)0x046B330, 0xC3);
	WriteData<1>((int*)0x04C0DC0, 0xC3);
	WriteData<1>((int*)0x0457D30, 0xC3);
	WriteData<1>((int*)0x071B210, 0xC3);
}


static void SetPlayerInitialPosition_r(taskwk* twp)
{
	if (!CheckRestartLevel() && CurrentLevel == destArray[count].lvl && CurrentAct == destArray[count].act)
	{
		twp->pos = destArray[count].pos;
		twp->ang.y = destArray[count].rotY;
	}
	else
	{
		SetPlayerInitialPosition_t.Original(twp);
	}
}


void SetPatches()
{
	HideHud_r();
	
	WriteData<1>((int*)0x4751B0, 0xC3); //remove death zone check (fix knux boundary)
	WriteData<5>(reinterpret_cast<void*>(0x00415007), static_cast<uint8_t>(0x90u)); //Prevents CurrentCharacter from being overwritten at initialization.
	WriteData<1>((int*)0x4AD140, 0xC3); //remove kiki
	WriteData<1>((int*)0x47D940, 0xC3); //remove Sonk AI

	WriteData<1>((int*)0x413B20, 0xC3);
	WriteData<5>(reinterpret_cast<void*>(0x5C0D67), static_cast<uint8_t>(0x90u)); //remove hardcoded cas start pos
	WriteData((uint8_t*)0x715350, static_cast<uint8_t>(0xC3u)); //remove chao world start pos
	WriteData<9>((int*)0x5C0D6C, 0x90);	//remove casino rot 
	WriteData<9>((int*)0x5C0DA0, 0x90);

	SetPlayerInitialPosition_t.Hook(SetPlayerInitialPosition_r);
}

void ProcessQuickBoot()
{
	//force the game to boot on a level
	WriteData(reinterpret_cast<GameModes*>(0x0040C10C), (GameModes)4);
	//add support for the code "skip intro" (force level boot instead of title screen, we will close the game after the screenshots so it doesn't really matter)
	WriteJump((void*)0x40C439, (void*)0x40C1E7);
	CurrentCharacter = character;
}

void initScreenshots()
{
	GenerateRandomLevelsLocation();
	SetPatches();
	ProcessQuickBoot(); //start the game and make it chain all levels generated
	LoadLevelObject_t.Hook(LoadLevelObject_r); //load a custom task that will generate our screenshot 
}