#include "pch.h"
#include "FunctionHook.h"

extern bool camHack;
extern uint8_t count;

static FunctionHook<void, taskwk*> SetPlayerInitialPosition_t(SetPlayerInitialPosition);
static FunctionHook<void, task*> Casino_HandleSpawnPoints_t((intptr_t)Casino_HandleSpawnPoints);

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

void SetCam()
{
	if (camera_twp)
	{
		CamOnFrames();
		SetCamForScreenshot();
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

void __cdecl Casino_HandleSpawnPoints_r(task* tp)
{
	auto twp = tp->twp;
	if (twp->mode != 2)
	{
		auto v2 = CreateElementalTask(LoadObj_Data1, 0, sub_5C03B0)->twp;

		camerahax_b();
		sub_5C0300();
		sub_5C04B0(0);
		SetViewAngle(12743);
		SetGravityMaybe(0, 0);
		v2->mode = 12;
		twp->mode = 2;
	}

	if (CurrentAct)
	{
		Casino_HandleSpawnPoints_t.Original(tp);
		return;
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
	WriteData<12>((int*)0x5C0D6C, 0x90);	//remove casino rot 
	WriteData<12>((int*)0x5C0DA0, 0x90);

	WriteData<1>((int*)0x4879B9, 0x1); //make amy always go boss victory pose
	Casino_HandleSpawnPoints_t.Hook(Casino_HandleSpawnPoints_r);

	for (uint8_t i = 5; i < 8; i++)
	{
		PhysicsArray[i] = PhysicsArray[Characters_Sonic];
	}


	SetPlayerInitialPosition_t.Hook(SetPlayerInitialPosition_r);
}
