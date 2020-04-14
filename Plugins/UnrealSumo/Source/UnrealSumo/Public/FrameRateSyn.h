#pragma once
#include "CoreMinimal.h"
#include "FrameRateSyn.generated.h"

USTRUCT(BlueprintType)
struct FrameRateSyn
{
	GENERATED_BODY()

public:
	/// Variables modified in MatchFrameRatePerSecond()
	// Unreal FPS and tick counter
	int32 TickCount = 0;
	int32 UnrealFPS = 0;

	///Variables modified in UpdateSUMOByMachineTime()
	// Unreal calculate when to Update form SUMO
	float NextTimeToUpdate = 0;
	float UpdateDeltaT = 0;
	int32 UETickBetweenSUMOUpdates = 0;

	void print() {
		UE_LOG(LogTemp, Warning, TEXT("TickCount: %d; UnrealFPS: %d; NextTimeToUpdate: %f; UpdateDeltaT: %f; UETickBetweenSUMOUpdates: %f"), TickCount, UnrealFPS, NextTimeToUpdate, UpdateDeltaT, UETickBetweenSUMOUpdates);
	}
};