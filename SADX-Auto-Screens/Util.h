#pragma once

ModelInfo* LoadBasicModel(const char* name);

ModelInfo* LoadChunkModel(const char* name);

void LoadModelListFuncPtr(const char** names, int count, ModelInfo** mdls, ModelInfo* (*func)(const char*));

void FreeMDL(ModelInfo* pointer);

void LoadAnimation(AnimationFile** info, const char* name, const HelperFunctions& helperFunctions);

inline AnimationFile* LoadANM(const char* type, const char* name);

void LookAt(NJS_VECTOR* unit, Angle* outx, Angle* outy);
void LookAt(NJS_VECTOR* from, NJS_VECTOR* to, Angle* outx, Angle* outy);
void PlayerLookAt(NJS_VECTOR* from, NJS_VECTOR* to, Angle* outx, Angle* outy);

bool IsPathExist(const std::string& s);

enum class CameraAdjustsIDs {
	None,
	Normal,
	Normal_S,
	Slow,
	Slow_S,
	Time,
	Three1,
	Three1C,
	Three2,
	Three2C,
	Three3,
	Three3C,
	Three4,
	Three4C,
	Three5,
	Three5C,
	Relative1,
	Relative1C,
	Relative2,
	Relative2C,
	Relative3,
	Relative3C,
	Relative4,
	Relative4C,
	Relative5,
	Relative5C,
	Relative6,
	FreeCam
};

enum class CameraDirectIDs {
	Sonic,
	Unknown,
	Target
};

static const void* const sub_5C04B0_ptr = (void*)0x5C04B0;
static inline void sub_5C04B0(int a1)
{
	__asm
	{
		mov eax, [a1]
		call sub_5C04B0_ptr
	}
}

DataPointer(_camcontwk, CameraTask, 0x3B2C660);
typedef void(__cdecl* CameraFuncPtr)(_OBJ_CAMERAPARAM*);
FunctionPointer(void, SetCameraEvent, (CameraFuncPtr func, CameraAdjustsIDs adjust, CameraDirectIDs direct), 0x437D20);
FunctionPointer(void, SetCameraMode_, (int mode), 0x4348F0);
VoidFunc(sub_5C0300, 0x5C0300);
TaskFunc(sub_5C03B0, 0x5C03B0);

DataPointer(NJS_ACTION, action_s_s0059_sonic, 0x3C84EF4);
DataPointer(NJS_ACTION, action_m_m0001_miles, 0x3C848C0);
DataPointer(NJS_ACTION, action_k_k0039_knuckles, 0x3C84980);
DataPointer(NJS_ACTION, action_k_k0013_knuckles, 0x3C85198);
DataPointer(NJS_ACTION, action_a_a0826_amy, 0x3C84AF0);
//EV_SetAction(player, BIG_ACTIONS[23], &BIG_TEXLIST, 1.4f, 1, 0);
//EV_SetAction(player, E102_ACTIONS[2], &E102_TEXLIST, 0.60000002f, 1, 16);