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

DataPointer(_camcontwk, CameraTask, 0x3B2C660);
typedef void(__cdecl* CameraFuncPtr)(_OBJ_CAMERAPARAM*);
FunctionPointer(void, SetCameraEvent, (CameraFuncPtr func, CameraAdjustsIDs adjust, CameraDirectIDs direct), 0x437D20);
FunctionPointer(void, SetCameraMode_, (int mode), 0x4348F0);