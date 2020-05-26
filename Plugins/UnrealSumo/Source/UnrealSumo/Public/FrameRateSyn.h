#pragma once
#include "CoreMinimal.h"
#include "FrameRateSyn.generated.h"

USTRUCT(BlueprintType)
struct FrameRateSyn
{
	GENERATED_BODY()

public:

	bool UpdateByMachineTime = false; // 

	/// Variables modified in SumoGameMode->MatchFrameRatePerSecond()
	int32 SUMOFPS = 0;
	// Unreal FPS
	int32 UnrealFPS = 0;

	///Variables modified in SumoGameMode->UpdateSUMOByMachineTime()
	// Unreal calculate when to update form SUMO. UpdateDeltaT is the delta time between an update from SUMO. NextTimeToUpdate record the next time to update from SUMO.
	// Both of units are seconds.
	float NextTimeToUpdate = 0;
	float UpdateDeltaT = 0;

	// Tick counter to record frames in Unreal Engine since start the game. UETickBetweenSUMOUpdates is the constant variable record 
	int32 TickCount = 0;
	int32 UETickBetweenSUMOUpdates = 0;

	// Record whether frame rate of Unreal is slower than SUMO. If Unreal Tick slower than SUMO, the flag is true;.
	bool UnrealTickSlower;

	void print() {
		UE_LOG(LogTemp, Warning, TEXT("TickCount: %d; UnrealFPS: %d; NextTimeToUpdate: %f; UpdateDeltaT: %f; UETickBetweenSUMOUpdates: %f"), TickCount, UnrealFPS, NextTimeToUpdate, UpdateDeltaT, UETickBetweenSUMOUpdates);
	}
};