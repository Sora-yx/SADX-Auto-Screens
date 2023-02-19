#include "pch.h"
#include "FunctionHook.h"
#include <atlimage.h>
#include <ShellApi.h>

static FunctionHook<void> LoadLevelObject_t(LoadLevelObject);
uint8_t count = 0;
bool camHack = false;

static Photos screenArray[]
{
	{ LevelIDs_EmeraldCoast, 0, {-627, 116, 1186}, 16384, {-636, 123, 1196}, {64819, 58146, 0}, GameModes_Trial  },
	{ LevelIDs_EmeraldCoast, 0, { 4298, 25, 375}, 19006, {0}, {0}, GameModes_Trial },
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

Photos destArray[max] = { NULL };
std::vector<uint8_t> levelDuplicate;

static int prevRng = -1;
static char rng = 0;
static int timerWait = 120;
static NJS_ACTION actionS_0059Copy;

void SetCharacterAnim()
{
	if (!playertp[0] || (playertwp[0]->flag & 3) == 0)
	{
		timerWait = 200;
		return;
	}

	do
	{
		rng = rand() % 3;
	} while (rng == prevRng);

	EV_LookFree(playertp[0]);

	if (!rng)
	{
		timerWait = 120;

		if (!useSS)
			ForcePlayerAction(0, 20);

		switch (CurrentCharacter)
		{
		case Characters_Sonic:

			actionS_0059Copy.motion = action_s_s0059_sonic.motion;

			if (MetalSonicFlag)
			{
				actionS_0059Copy.object = SONIC_OBJECTS[58];
				EV_SetAction(playertp[0], &actionS_0059Copy, &METALSONIC_TEXLIST, 0.5f, 1, 3);
			}
			else
			{
				if (!useSS)
					EV_SetAction(playertp[0], &action_s_s0059_sonic, &SONIC_TEXLIST, 0.5f, 1, 16);
			}
			destArray[count].camPos.y -= 2.0f;
			playertwp[0]->ang.y += 0x2000;
			break;
		case Characters_Tails:
			EV_SetAction(playertp[0], &action_m_m0001_miles, &MILES_TEXLIST, 0.5f, 1, 16);
			break;
		case Characters_Knuckles:
			EV_SetFace(playertp[0], "VVVVVVVVVVVVVVVVVV");
			EV_SetAction(playertp[0], &action_k_k0039_knuckles, &KNUCKLES_TEXLIST, 0.5f, 1, 16);
			destArray[count].camPos.y -= 2.0f;
			playertwp[0]->ang.y += 0x500;
			break;
		case Characters_Amy:
			AmyForEventHammerScaleIm(0, 0.8f);
			EV_SetAction(playertp[0], &action_a_a0826_amy, &AMY_TEXLIST, 0.5f, 1, 16);
			playertwp[0]->ang.y += 0x4900;
			break;
		case Characters_Big:
			EV_SetAction(playertp[0], BIG_ACTIONS[23], &BIG_TEXLIST, 1.4f, 1, 0);
			break;
		case Characters_Gamma:
			EV_SetAction(playertp[0], E102_ACTIONS[2], &E102_TEXLIST, 0.60000002f, 1, 16);
			break;
		}
	}
	else if (rng == 1)
	{
		timerWait = CurrentCharacter == Characters_Amy ? 110 : 130;
		if (CurrentCharacter == Characters_Big)
		{
			playerpwp[0]->mj.reqaction = 1;
		}
		else if (CurrentCharacter == Characters_Gamma)
		{
			playerpwp[0]->mj.reqaction = 73;
		}
		else
		{
			playerpwp[0]->mj.reqaction = 4;
		}
	}
	else
	{
		timerWait = 180;
		SetTailsRaceVictory();
		ForcePlayerAction(0, 19);
	}

	prevRng = rng;
}

bool screenCapturePart(int x, int y, int w, int h, LPCSTR fname)
{
	CString transitionString = fname;
	LPCTSTR fnameW = transitionString;

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

	CImage image;
	image.Attach(hBitmap);

	if (SUCCEEDED(image.Save(fnameW)))
	{
		return true;
	}

	return false;
}

void SetScreenShot(task* tp)
{
	SetCam();

	//if we aren't in game or if a cutscene is running, abort
	if (GameState != 15 || EV_MainThread_ptr)
		return;

	auto twp = tp->twp;
	std::string folder = modpath + "\\screenshots";
	std::string path = folder + "\\" + std::to_string(count) + ".jpg";
	int crop = IsWindowed ? 15 : 0;
	bool camDead = destArray[count].camPos.x == 0 && destArray[count].camPos.y == 0 && destArray[count].camPos.z == 0;

	switch (twp->mode)
	{
	case 0:
		DisablePause();
		DisableControl();
		EV_ClrFace(playertp[0]);
		ForcePlayerAction(0, useSS ? 46 : 24);
		playerpwp[0]->item |= Powerups_Invincibility;
	
		if (!camDead)
		{
			camHack = true;
			SetCharacterAnim();
		}
			
		twp->mode++;
		break;
	case 1:
		if (++twp->wtimer == timerWait) //wait one second
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
		if (++twp->wtimer == 20)
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
	GameMode = destArray[count].gameMode;
	CreateElementalTask(2, 2, SetScreenShot);
}

void ProcessQuickBoot()
{
	//force the game to boot on a level
	WriteData(reinterpret_cast<GameModes*>(0x0040C10C), (GameModes)4);
	//add support for the code "skip intro" (force level boot instead of title screen, we will close the game after the screenshots so it doesn't really matter)
	WriteJump((void*)0x40C439, (void*)0x40C1E7);
	CurrentCharacter = character;
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

		if (CurrentCharacter >= Characters_Gamma)
		{
			destArray[i].camPos.x += 5.0f;
			destArray[i].camPos.y += 8.0f;
			destArray[i].camPos.z += 10.0f;
		}
	}
}

void initScreenshots()
{
	ProcessQuickBoot(); //start the game and make it chain all levels generated
	GenerateRandomLevelsLocation();
	SetPatches();
	LoadLevelObject_t.Hook(LoadLevelObject_r); //load a custom task that will generate our screenshot 
}