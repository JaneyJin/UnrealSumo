#pragma once
#include "CoreMinimal.h"
#include "FrameRateSyn.generated.h"

USTRUCT(BlueprintType)
struct FrameRateSyn
{
	GENERATED_BODY()

public:

	bool UpdateByMachineTime = false; // 

	/// Variables modified in MatchFrameRatePerSecond()
	int32 SUMOFPS = 0;
	// Unreal FPS and tick counter
	int32 TickCount = 0;
	int32 UnrealFPS = 0;

	///Variables modified in UpdateSUMOByMachineTime()
	// Unreal calculate when to Update form SUMO
	float NextTimeToUpdate = 0;
	float UpdateDeltaT = 0;
	int32 UETickBetweenSUMOUpdates = 0;
};